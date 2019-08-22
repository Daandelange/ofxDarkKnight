/*
 Copyright (C) 2019 Luis Fernando Garc�a [http://luiscript.com]

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

#ifndef ParamInverter_hpp
#define ParamInverter_hpp

#include "module.hpp"

class ParamInverter : public Module
{
private:
	float source;
	float target;
public:
	void setup() 
	{
		target = source = 0.0;
	}
	void update() 
	{
		target = 1.0 - source;
	}
	void addModuleParameters()
	{
		gui->addSlider("source", 0.0, 1.0, 1.0)->setPrecision(4)->bind(source);
		gui->addSlider("target", 0.0, 1.0, 0.0)->setPrecision(4)->bind(target);
	}
};

#endif /* Inverter_hpp */
