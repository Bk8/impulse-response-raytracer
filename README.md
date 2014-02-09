impulse-response-raytracer
==========================

raytracing impulse response designer

There are three main things you need to worry about. The raytracing core lib is in /librayverb, the obj loader and tracer is in /tracer, and the trace renderer is in /flattener.

I'm not sure if this builds not-on-my-machine yet. To build, you'll need a copy of the JUCE library. I've added JUCE as a submodule, but haven't got round to correcting all the paths properly yet. You might have to some mucking around to get JUCE building properly.

I've also added Assimp as a submodule, but this is more for convenience when downloading the source. You'll still need to manually build the Assimp library, which means you'll also need CMake.

Maybe if I'm feeling brave I'll convert the entire build system over to CMake at some point.

For the moment, attempting to build this is not for the faint of heart, or those with lots of important things to do.
