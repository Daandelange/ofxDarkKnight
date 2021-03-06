/*
 Copyright (C) 2019 Luis Fernando García Pérez [http://luiscript.com]
 
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

#include "DKLua.h"

void DKLua::setup()
{
    loaded = gotTexture = false;

    
    fbo = new ofFbo;
    fbo->allocate(getModuleWidth(), getModuleHeight(), GL_RGBA);
    fbo->begin();
    ofClear(0, 0, 0, 255);
    fbo->end();
    
    parameterName = "";
    min = 0.0;
    max = 1.0;
    precision = 2;
    
    addOutputConnection(DKConnectionType::DK_FBO);
    fileChangedTimes.clear();
    
    ofAddListener(fileDialog.fileDialogEvent, this, &DKLua::onFileDialogResponse);
    lastTimeCheckMillis = ofGetElapsedTimeMillis();
}

void DKLua::update()
{
    if(loaded) {
        if( loadShaderNextFrame )
        {
            reloadScript();
            loadShaderNextFrame = false;
        }
        int currTime = ofGetElapsedTimeMillis();
        
        if( ((currTime - lastTimeCheckMillis) > millisBetweenFileCheck) &&
           !loadShaderNextFrame )
        {

            if( filesChanged() )
            {
                loadShaderNextFrame = true;
            }
     
            lastTimeCheckMillis = currTime;
        }
        for (auto& it : floatParameters)
        {
            lua.setNumber(it.first, *it.second);
        }
        for (auto& it : intParameters)
        {
            lua.setNumber(it.first, *it.second);
        }
        lua.scriptUpdate();
    }
}

void DKLua::draw()
{
    if (loaded)
    {
        fbo->begin();
        lua.scriptDraw();
        fbo->end();
    }
}


void DKLua::addModuleParameters()
{
    ofxDatGuiFolder* DKLuaSettings = gui->addFolder("SCRIPT SETTINGS");
    auto newButtton = DKLuaSettings->addButton("New");
    auto openButton = DKLuaSettings->addButton("Open");
    auto reloadButton = DKLuaSettings->addButton("Reload");
    
    newButtton->onButtonEvent(this, &DKLua::onSettingsButtonPress);
    openButton->onButtonEvent(this, &DKLua::onSettingsButtonPress);
    reloadButton->onButtonEvent(this, &DKLua::onSettingsButtonPress);
    
    ofxDatGuiFolder* addParameter = gui->addFolder("ADD PARAMETER");
    ofxDatGuiTextInput* parameterName = addParameter->addTextInput("Name", "parameter");
    parameterName->setTextUpperCase(false);
    parameterName->onTextInputEvent(this, &DKLua::onParameterNameChange);
    ofxDatGuiTextInput* parameterMin = addParameter->addTextInput("Min", "0");
    parameterMin->onTextInputEvent(this, &DKLua::onParameterMinChange);
    ofxDatGuiTextInput* parameterMax = addParameter->addTextInput("Max", "1");
    parameterMax->onTextInputEvent(this, &DKLua::onParameterMaxChange);
    ofxDatGuiTextInput* parameterPrecision = addParameter->addTextInput("Decimals", "2");
    parameterPrecision->onTextInputEvent(this, &DKLua::onParameterPrecisionChange);
    
    ofxDatGuiButton* addParameterButton = addParameter->addButton("ADD");
    addParameterButton->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    addParameterButton->onButtonEvent(this, &DKLua::addParameter);
    
    params = gui->addFolder("PARAMETERS");
    
}

void DKLua::onSettingsButtonPress(ofxDatGuiButtonEvent e)
{
    if (e.target->getName() == "New")
    {
        fileDialog.setup();
        fileDialog.saveFile("New", "Create new script", "live.lua");
    
    }
    else if (e.target->getName() == "Open")
    {
        fileDialog.setup();
        fileDialog.openFile("Open", "Open script");
    }
    else if( e.target->getName() == "Reload Script")
    {
        reloadScript();
    }
}

void DKLua::onFileDialogResponse(ofxThreadedFileDialogResponse& response) {
    fileDialog.stop();
    
    if (response.id == "Open")
    {
        script = response.filepath;
        scriptFile = ofFile(script);
        
        if(scriptFile.exists())
        {
            fileChangedTimes.push_back( getLastModified( scriptFile ) );
            lua.addListener(this);
            reloadScript();
            lastTimeCheckMillis = ofGetElapsedTimeMillis();
            setMillisBetweenFileCheck( 200 );
            loaded = true;
        }
    }
    else if (response.id == "New")
    {
        const GLchar* luas =
        R"END(function draw()
of.clear(0)
end)END";
        script = response.filepath;
        ofstream dest(script.c_str(), ios::binary);
        dest << luas;
        scriptFile = ofFile(script);
        
        if(scriptFile.exists())
        {
            fileChangedTimes.push_back( getLastModified( scriptFile ) );
            lua.addListener(this);
            reloadScript();
            lastTimeCheckMillis = ofGetElapsedTimeMillis();
            setMillisBetweenFileCheck( 200 );
            loaded = true;
        }
    }
}

void DKLua::reloadScript()
{
    lua.init();
    lua.doScript(script, true);
    lua.scriptSetup();
    loaded = true;
}

void DKLua::addParameter(ofxDatGuiButtonEvent e)
{
    if(parameterName != "")
    {
        if(precision > 0)
        {
            float * newFloatParam = new float;
            ofxDatGuiSlider * newSlider;
            newSlider = params->addSlider(parameterName, min, max);
            newSlider->setPrecision(precision)->bind(*newFloatParam);
            if(getModuleMidiMapMode() != newSlider->getMidiMode())
            {
                newSlider->toggleMidiMode();
            }
            bind(*newFloatParam);
            floatParameters.insert({parameterName, newFloatParam});
        } else {
            int * newIntParam = new int;
            ofxDatGuiSlider * newSlider;
            newSlider = params->addSlider(parameterName, min, max);
            newSlider->setPrecision(precision)->bind(*newIntParam);
            if(getModuleMidiMapMode() != newSlider->getMidiMode())
            {
                newSlider->toggleMidiMode();
            }
            bind(*newIntParam);
            intParameters.insert({parameterName, newIntParam});
        }
        gui->setWidth(moduleGuiWidth);
    }
}

void DKLua::onParameterNameChange(ofxDatGuiTextInputEvent e)
{
    parameterName = e.target->getText();
}

void DKLua::onParameterMinChange(ofxDatGuiTextInputEvent e)
{
    min = ofToFloat(e.target->getText());
}

void DKLua::onParameterMaxChange(ofxDatGuiTextInputEvent e)
{
    max = ofToFloat(e.target->getText());
}

void DKLua::onParameterPrecisionChange(ofxDatGuiTextInputEvent e)
{
    precision = ofToInt(e.target->getText());
}

ofFbo * DKLua::getFbo()
{
    return fbo;
}

void DKLua::errorReceived(string& msg)
{
    lua.scriptExit();
    lua.clear();
}


bool DKLua::filesChanged()
{
    bool fileChanged = false;
    
    if( scriptFile.exists() )
    {
        time_t scriptChangeTime = getLastModified( scriptFile );
        if( scriptChangeTime != fileChangedTimes.at(0) )
        {
            fileChangedTimes.at(0) = scriptChangeTime;
            fileChanged = true;
        }
    }
    
    return fileChanged;
}


std::time_t DKLua::getLastModified( ofFile& _file )
{
    if( _file.exists() )
    {
        return filesystem::last_write_time(_file.path());
    }
    else
    {
        return 0;
    }
}


void DKLua::setMillisBetweenFileCheck( int _millis )
{
    millisBetweenFileCheck = _millis;
}


void DKLua::unMount()
{
    ofRemoveListener(fileDialog.fileDialogEvent, this, &DKLua::onFileDialogResponse);
}
