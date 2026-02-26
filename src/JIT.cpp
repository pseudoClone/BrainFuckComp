#include "JIT.hpp"
#include <stdexcept>
#include <stack>
#include <sys/mman.h>
#include <cstring>
#include <dlfcn.h>
#include <unordered_map>
#include <tuple>

JIT::BF_OPERATION JIT::charToOP(char c)
{
        switch (c)
        {
        case '>':
                return JIT::BF_OPERATION::OP_MOVE_RIGHT;
        case '<':
                return JIT::BF_OPERATION::OP_MOVE_LEFT;
        case '+':
                return JIT::BF_OPERATION::OP_INC;
        case '-':
                return JIT::BF_OPERATION::OP_DEC;
        case '.':
                return JIT::BF_OPERATION::OP_WRITE;
        case ',':
                return JIT::BF_OPERATION::OP_READ;
        case '[':
                return JIT::BF_OPERATION::OP_JZ;
        case ']':
                return JIT::BF_OPERATION::OP_JNZ;
        default:
                throw std::invalid_argument("Invalid BF Keyword");
        }
}

void JIT::parse(const std::string &src)
{
        ir.clear(); // Remove if existing. I fear buffer overflows
        std::string clean{};
        for (auto c : src)
        {
                // Basically Python equivalent of if(c not in "+-><.,[]")
                // npos denotes there exist no position for the given character in the string container
                if (std::string("><+-.,[]").find(c) != std::string::npos)
                {
                        clean.push_back(c);
                }
        }
        std::vector<size_t> bracketMatch(clean.size(), 0);
        std::stack<size_t> bracketStack{};

        for (size_t i = 0; i < clean.size(); ++i)
        {
                if (clean[i] == '[')
                {
                        bracketStack.push(i);
                }
                else if (clean[i] == ']')
                {
                        if (bracketStack.empty())
                        {
                                throw std::runtime_error("Error: Unmatched ]");
                        }
                        size_t open = bracketStack.top();
                        bracketMatch[open] = i;
                        bracketMatch[i] = open;
                        bracketStack.pop();
                }
        }
        if (!bracketStack.empty())
        {
                throw std::runtime_error("Error: Unmatched brackets");
        }

        for (size_t i = 0; i < clean.size();)
        {
                // We will increase i based on RLE and not one at a time because we don't know if count is 0 or 1, 2, etc.
                char c = clean[i];
                BF_OPERATION op = charToOP(c);

                if (op == JIT::BF_OPERATION::OP_INC || op == JIT::BF_OPERATION::OP_DEC || op == JIT::BF_OPERATION::OP_MOVE_LEFT || op == JIT::BF_OPERATION::OP_MOVE_RIGHT)
                {
                        size_t count_op = 1;
                        while (i + count_op < clean.size() && clean[i + count_op] == c)
                        {
                                ++count_op;
                        }
                        ir.push_back({op, count_op});
                        i += count_op;
                }
                else if (op == JIT::BF_OPERATION::OP_JZ || op == JIT::BF_OPERATION::OP_JNZ)
                {
                        ++i; // Fuck optimization, jump bracket computation is optimized enough
                        continue;
                }
                else
                {
                        ir.push_back({op, 0});
                        ++i;
                }
        }
}

void JIT::optimize()
{
        std::vector<IR_MNEMONIC> out;
        for (size_t i = 0; i < ir.size();)
        {
                if (i + 1 < ir.size())
                {
                        auto &a = ir[i], &b = ir[i + 1];
                        if ((a.op == JIT::BF_OPERATION::OP_INC && b.op == JIT::BF_OPERATION::OP_DEC) ||
                            (a.op == JIT::BF_OPERATION::OP_DEC && b.op == JIT::BF_OPERATION::OP_INC))
                        {
                                i += 2;
                                continue;
                        }
                        if ((a.op == JIT::BF_OPERATION::OP_MOVE_RIGHT && b.op == JIT::BF_OPERATION::OP_MOVE_LEFT) ||
                            (a.op == JIT::BF_OPERATION::OP_MOVE_LEFT && b.op == JIT::BF_OPERATION::OP_MOVE_RIGHT))
                        {
                                i += 2;
                                continue;
                        }
                }
                out.push_back(ir[i++]);
        }
        ir = std::move(out);
}

void *JIT::emit()
{
        std::vector<uint8_t> code;
        std::unordered_map<size_t, size_t> srcIdxToCodeOffset;

        code.insert(code.end(), {0x55, 0x48, 0x89, 0xe5});
        code.insert(code.end(), {0x53, 0x56, 0x57});

        void *putchar_addr = dlsym(RTLD_DEFAULT, "putchar");
        void *getchar_addr = dlsym(RTLD_DEFAULT, "getchar");

        size_t src_idx = 0;
        for (size_t ir_idx = 0; ir_idx < ir.size(); ++ir_idx)
        {
                auto &instr = ir[ir_idx];

                if (instr.op == JIT::BF_OPERATION::OP_JZ || instr.op == JIT::BF_OPERATION::OP_JNZ)
                {
                        srcIdxToCodeOffset[src_idx] = code.size();
                        src_idx++;
                }
                else if (instr.op == JIT::BF_OPERATION::OP_INC ||
                         instr.op == JIT::BF_OPERATION::OP_DEC ||
                         instr.op == JIT::BF_OPERATION::OP_MOVE_RIGHT ||
                         instr.op == JIT::BF_OPERATION::OP_MOVE_LEFT)
                {
                        for (size_t k = 0; k < instr.non_immediate_arg; ++k)
                        {
                                srcIdxToCodeOffset[src_idx] = code.size();
                                src_idx++;
                        }
                }
                else
                {
                        srcIdxToCodeOffset[src_idx] = code.size();
                        src_idx++;
                }

                switch (instr.op)
                {
                case JIT::BF_OPERATION::OP_MOVE_RIGHT:
                        for (size_t j = 0; j < instr.non_immediate_arg; ++j)
                                code.insert(code.end(), {0x48, 0xff, 0xc7});
                        break;
                case JIT::BF_OPERATION::OP_MOVE_LEFT:
                        for (size_t j = 0; j < instr.non_immediate_arg; ++j)
                                code.insert(code.end(), {0x48, 0xff, 0xcf});
                        break;
                case JIT::BF_OPERATION::OP_INC:
                        if (instr.non_immediate_arg == 1)
                                code.insert(code.end(), {0xfe, 0x07});
                        else
                                code.insert(code.end(), {0x80, 0x07, static_cast<uint8_t>(instr.non_immediate_arg)});
                        break;
                case JIT::BF_OPERATION::OP_DEC:
                        if (instr.non_immediate_arg == 1)
                                code.insert(code.end(), {0xfe, 0x2f});
                        else
                                code.insert(code.end(), {0x80, 0x2f, static_cast<uint8_t>(instr.non_immediate_arg)});
                        break;
                case JIT::BF_OPERATION::OP_WRITE:
                        code.insert(code.end(), {0x48, 0x83, 0xEC, 0x08, 0x0f, 0xb6, 0x07, 0x89, 0xc7});
                        code.push_back(0xe8);
                        code.insert(code.end(), {0x00, 0x00, 0x00, 0x00});
                        {
                                intptr_t base = reinterpret_cast<intptr_t>(code.data());
                                intptr_t target = reinterpret_cast<intptr_t>(putchar_addr);
                                int32_t rel = static_cast<int32_t>(target - (base + code.size()));
                                std::memcpy(code.data() + code.size() - 4, &rel, 4);
                        }
                        code.insert(code.end(), {0x48, 0x83, 0xC4, 0x08, 0x31, 0xc0});
                        break;
                case JIT::BF_OPERATION::OP_READ:
                        code.push_back(0xe8);
                        code.insert(code.end(), {0x00, 0x00, 0x00, 0x00});
                        {
                                intptr_t base = reinterpret_cast<intptr_t>(code.data());
                                intptr_t target = reinterpret_cast<intptr_t>(getchar_addr);
                                int32_t rel = static_cast<int32_t>(target - (base + code.size()));
                                std::memcpy(code.data() + code.size() - 4, &rel, 4);
                        }
                        code.insert(code.end(), {0x88, 0x07});
                        break;
                case JIT::BF_OPERATION::OP_JZ:
                case JIT::BF_OPERATION::OP_JNZ:
                        code.insert(code.end(), {0x80, 0x3f, 0x00});
                        code.push_back(instr.op == JIT::BF_OPERATION::OP_JZ ? 0x74 : 0x75);
                        code.push_back(0x00);
                        break;
                }
        }

        code.insert(code.end(), {0x5f, 0x5e, 0x5b, 0x5d, 0xc3});

        void *exec = mmap(nullptr, code.size(),
                          PROT_READ | PROT_WRITE | PROT_EXEC,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (exec == MAP_FAILED)
                throw std::runtime_error("mmap failed");

        std::memcpy(exec, code.data(), code.size());

        uint8_t *exec_bytes = static_cast<uint8_t *>(exec);
        for (size_t ir_idx = 0; ir_idx < ir.size(); ++ir_idx)
        {
                auto &instr = ir[ir_idx];
                if (instr.op == JIT::BF_OPERATION::OP_JZ || instr.op == JIT::BF_OPERATION::OP_JNZ)
                {
                        size_t target_src_idx = instr.non_immediate_arg;
                        auto it = srcIdxToCodeOffset.find(target_src_idx);
                        if (it != srcIdxToCodeOffset.end())
                        {
                                size_t target_code_off = it->second;
                                size_t jump_instr_off = 0;
                                size_t count = 0;
                                for (size_t i = 0; i <= ir_idx; ++i)
                                {
                                        if (ir[i].op == JIT::BF_OPERATION::OP_INC ||
                                            ir[i].op == JIT::BF_OPERATION::OP_DEC ||
                                            ir[i].op == JIT::BF_OPERATION::OP_MOVE_RIGHT ||
                                            ir[i].op == JIT::BF_OPERATION::OP_MOVE_LEFT)
                                        {
                                                count += ir[i].non_immediate_arg;
                                        }
                                        else
                                        {
                                                count++;
                                        }
                                        if (i == ir_idx)
                                                break;
                                }
                                jump_instr_off = srcIdxToCodeOffset[count - 1] + 2;
                                int32_t disp = static_cast<int32_t>(target_code_off) - static_cast<int32_t>(jump_instr_off);
                                if (disp >= -128 && disp <= 127)
                                {
                                        exec_bytes[jump_instr_off] = static_cast<uint8_t>(disp);
                                }
                        }
                }
        }

        if (mprotect(exec, code.size(), PROT_READ | PROT_EXEC) == -1)
        {
                munmap(exec, code.size());
                throw std::runtime_error("mprotect failed");
        }

        return exec;
}