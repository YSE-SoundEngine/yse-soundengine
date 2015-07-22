/*
  ==============================================================================

    source.h
    Created: 31 Jan 2014 2:53:05pm
    Author:  yvan

  ==============================================================================
*/

#ifndef SOURCE_H_INCLUDED
#define SOURCE_H_INCLUDED


#include <vector>
#include "buffer.hpp"
#include "../headers/enums.hpp"

namespace YSE {
  namespace SOUND {
    class implementationObject;
  }

  namespace DSP {

    // simple base class for a chainable dsp object

    class API dspObject {
    public:
      dspObject();
      virtual ~dspObject();

      // the create function can be used to allocate memory on the DLL heap
      void createIfNeeded();
      virtual void create() = 0;
      virtual void process(MULTICHANNELBUFFER & buffer) = 0;

      // link the output of this dsp to another dsp.
      // If the dsp is already linked, the new dsp will
      // be put between this object and the current next
      // object.
      void link(dspObject& next);
      dspObject * link();

      dspObject& bypass(Bool value) { _bypass = value; return *this; }
      Bool       bypass() { return _bypass; }

      dspObject ** calledfrom; // consider this private for now

    private:
      dspObject * next;
      dspObject * previous;
      Bool _bypass;
      Bool _needsCreate;
    };

    // simple base class for a dsp object with sound generation
    // DSPSource can be by sounds for sound generation. This is
    // why some virtual functions have to be implemented.
    class API dspSourceObject {
    public:
      std::vector<buffer> buffer;
      dspSourceObject(Int buffers = 1);

      // intent is what we should do (playing, start playing, start stopping etc...
      virtual void process(SOUND_STATUS & intent) = 0;
      virtual void frequency(Flt value) = 0;
    };

  }
}




#endif  // SOURCE_H_INCLUDED
