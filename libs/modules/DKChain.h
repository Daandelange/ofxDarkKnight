/*
 Copyright (C) 2019 Luis Fernando Garc�a P�rez [http://luiscript.com]

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef DKChain_h
#define DKChain_h

#include "DKModule.hpp"

class DKChain : public DKModule
{
public:
    void setup();
    void update();
    void draw();
    ofFbo* getFbo();
    void setFbo(ofFbo*);
private:

    void processChain(ofFbo&, ofFbo&, DKModule*);
    unsigned currentReadFbo;
    unsigned numFx = 0;
    bool gotTexture;
    
    ofFbo raw;
    ofFbo* fboIn;
    ofFbo pingPong[2];
};

#endif /* DKChain_h */
