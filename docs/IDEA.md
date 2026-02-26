# Idea

I was thinking about how JIT compilers actually work. I mean, everyone knows how JIT compilers work. But in reality not everyone makes one. So, I asked Gemini first, how does it work. It told me about all the hotspot stuff and profiling and system monitoring. So, I thought, why not make one.

And in all honesty, I haven't made even a single traditional compiler in my life. I tried once, to make a compiler for an arbritary language in C using a tutorial 3 years ago. Also tried to make a self hosted C compiler. Got hyped up and even emailed Richard Stallman asking for the codes of the first version(the ones even before 1986 where he was just starting to write about the RTL stuff).

So, I thought why not make it in BrainFuck. I mean the language is simple. I can make a simple interpreter in 10 minutes and all I have to do really is make the components of the JIT compiler. 

Then, I thought, no of my ideas are original. And low and behold, my favourite Youtube Video Creator, Tsoding had made one. Hell, there were like blog posts(with 2 parts) making one.

Nevertheless, I thought, fuck it, I am making one.


## 26th February, 2026
Yesterday I was looking into GNU Lightning. It's simple. It's lightweight and give me machine instructions and supports x86-64 by default. So, why the hell not. I suck at anything that is in LLVM level. Hats off to Chris Lattner but what the fuck bro. So, may as well look into something that is small, does the job. And no optimization layer, I'll do that part.