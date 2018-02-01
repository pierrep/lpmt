#include "ofApp.h"

#ifdef WITH_MIDI

// MIDI message callback

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {

	// make a copy of the latest message
	midiMessage = msg;
	// following is for debug purposes
	/*
	cout << "status: " << midiMessage.status << endl;
	cout << "status string: " << midiMessage.getStatusString(midiMessage.status) << endl;
	cout << "channel: " << midiMessage.channel << endl;
	cout << "pitch: " << midiMessage.pitch << endl;
	cout << "velocity: " << midiMessage.velocity << endl;
	cout << "control: " << midiMessage.control << endl;
	cout << "value: " << midiMessage.value << endl;
	*/

	if(bMidiHotkeyCoupling && midiHotkeyPressed >= 0)
    {
        if(midiHotkeyMessages.size()>0 && midiHotkeyMessages.size() == midiHotkeyKeys.size())
        {
        for(int i=0; i < midiHotkeyMessages.size(); i++)
        {
            if(midiHotkeyKeys[i] == midiHotkeyPressed)
            {
                midiHotkeyKeys.erase(midiHotkeyKeys.begin()+i);
                midiHotkeyMessages.erase(midiHotkeyMessages.begin()+i);
            }
        }
        }
        midiHotkeyMessages.push_back(midiMessage);
        midiHotkeyKeys.push_back(midiHotkeyPressed);
        midiHotkeyPressed = -1;
        bMidiHotkeyCoupling = false;
        bMidiHotkeyLearning = false;
        return;
    }

	if(midiHotkeyMessages.size()>0 && midiHotkeyMessages.size() == midiHotkeyKeys.size())
    {
        for(int i=0; i < midiHotkeyMessages.size(); i++)
        {
            ofxMidiMessage midiControl = midiHotkeyMessages[i];
            if(midiMessage.velocity >0 && midiMessage.status == midiControl.status && midiMessage.pitch == midiControl.pitch && midiMessage.channel == midiControl.channel)
            {
                keyPressed(midiHotkeyKeys[i]);
            }
        }
    }


	for(int i=0; i < m_gui.getPages().size(); i++)
	{
	    for(int j=0; j < m_gui.getPages()[i]->getControls().size(); j++)
	    {
	        if(m_gui.getPages()[i]->getControls()[j]->controlType == "Toggle")
	        {
                if(m_gui.getPages()[i]->getControls()[j]->bLearning)
                {
                m_gui.getPages()[i]->getControls()[j]->bLearning = false;
                m_gui.getPages()[i]->getControls()[j]->bLearnt = true;
                m_gui.getPages()[i]->getControls()[j]->midiControl = midiMessage;
                }
                else if(m_gui.getPages()[i]->getControls()[j]->bLearnt)
                {
                    ofxMidiMessage midiControl = m_gui.getPages()[i]->getControls()[j]->midiControl;
                    if(midiMessage.status == midiControl.status && midiMessage.pitch == midiControl.pitch && midiMessage.velocity == midiControl.velocity && midiMessage.channel == midiControl.channel)
                    {
                    ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle *) m_gui.getPages()[i]->getControls()[j];
                    t->toggle();
                    }
                }
	        }
	        else if(m_gui.getPages()[i]->getControls()[j]->controlType == "SliderFloat" || m_gui.getPages()[i]->getControls()[j]->controlType == "SliderInt")
	        {
	            if(m_gui.getPages()[i]->getControls()[j]->bLearning)
                {
                    if(midiMessage.status == MIDI_CONTROL_CHANGE || midiMessage.status == MIDI_PITCH_BEND)
                    {
                        m_gui.getPages()[i]->getControls()[j]->bLearning = false;
                        m_gui.getPages()[i]->getControls()[j]->bLearnt = true;
                        m_gui.getPages()[i]->getControls()[j]->midiControl = midiMessage;
                    }
                    else if(midiMessage.status == MIDI_NOTE_ON && midiMessage.velocity > 0)
                    {
                        if(m_gui.getPages()[i]->getControls()[j]->midiSlideControls.size() == 0)
                        {
                            m_gui.getPages()[i]->getControls()[j]->midiSlideControls.push_back(midiMessage);
                        }
                        else if(m_gui.getPages()[i]->getControls()[j]->midiSlideControls.size() == 1)
                        {
                            m_gui.getPages()[i]->getControls()[j]->midiSlideControls.push_back(midiMessage);
                            m_gui.getPages()[i]->getControls()[j]->bLearning = false;
                            m_gui.getPages()[i]->getControls()[j]->bLearnt = true;
                        }
                    }
                }
                else if(m_gui.getPages()[i]->getControls()[j]->bLearnt)
                {
                    ofxMidiMessage midiControl = m_gui.getPages()[i]->getControls()[j]->midiControl;
                    vector<ofxMidiMessage> midiSlideControls = m_gui.getPages()[i]->getControls()[j]->midiSlideControls;

                    if(midiMessage.status == MIDI_CONTROL_CHANGE)
                    {
                        if(midiMessage.status == midiControl.status && midiMessage.control == midiControl.control && midiMessage.channel == midiControl.channel)
                        {
                            if(m_gui.getPages()[i]->getControls()[j]->controlType == "SliderFloat")
                            {
                                ofxSimpleGuiSliderFloat *s = (ofxSimpleGuiSliderFloat *) m_gui.getPages()[i]->getControls()[j];
                                float value = midiMessage.value;
                                float remappedValue = ofMap(value, (float) 0, (float) 127, (float) s->min, (float) s->max);
                                s->setValue(remappedValue);
                            }
                            else
                            {
                                ofxSimpleGuiSliderInt *s = (ofxSimpleGuiSliderInt *) m_gui.getPages()[i]->getControls()[j];
                                float value = midiMessage.value;
                                float remappedValue = ofMap(value, (float) 0, (float) 127, (float) s->min, (float) s->max);
                                s->setValue((int)remappedValue);
                            }
                        }
                    }
                    else if(midiMessage.status == MIDI_PITCH_BEND)
                    {
                        if(midiMessage.status == midiControl.status && midiMessage.channel == midiControl.channel)
                        {
                            if(m_gui.getPages()[i]->getControls()[j]->controlType == "SliderFloat")
                            {
                                ofxSimpleGuiSliderFloat *s = (ofxSimpleGuiSliderFloat *) m_gui.getPages()[i]->getControls()[j];
                                float value = midiMessage.value;
                                float remappedValue = ofMap(value, (float) 0, (float) 16383, (float) s->min, (float) s->max);
                                s->setValue(remappedValue);
                            }
                            else
                            {
                                ofxSimpleGuiSliderInt *s = (ofxSimpleGuiSliderInt *) m_gui.getPages()[i]->getControls()[j];
                                float value = midiMessage.value;
                                float remappedValue = ofMap(value, (float) 0, (float) 16383, (float) s->min, (float) s->max);
                                s->setValue((int)remappedValue);
                            }
                        }
                    }
                    else if(midiMessage.status == MIDI_NOTE_ON && midiMessage.velocity > 0)
                    {
                        if(midiSlideControls.size() == 2)
                        {
                           if (midiMessage.status == midiSlideControls[0].status && midiMessage.pitch == midiSlideControls[0].pitch && midiMessage.channel == midiSlideControls[0].channel)
                           {
                                if(m_gui.getPages()[i]->getControls()[j]->controlType == "SliderFloat")
                                {
                                    ofxSimpleGuiSliderFloat *s = (ofxSimpleGuiSliderFloat *) m_gui.getPages()[i]->getControls()[j];
                                    //s->decrease();
                                    s->setValue(s->getValue()-(s->max - s->min)*0.002);
                                }
                                else
                                {
                                    ofxSimpleGuiSliderInt *s = (ofxSimpleGuiSliderInt *) m_gui.getPages()[i]->getControls()[j];
                                    //s->decrease();
                                    s->setValue(s->getValue()-1);
                                }
                           }
                           else if (midiMessage.status == midiSlideControls[1].status && midiMessage.pitch == midiSlideControls[1].pitch && midiMessage.channel == midiSlideControls[1].channel)
                           {
                                if(m_gui.getPages()[i]->getControls()[j]->controlType == "SliderFloat")
                                {
                                    ofxSimpleGuiSliderFloat *s = (ofxSimpleGuiSliderFloat *) m_gui.getPages()[i]->getControls()[j];
                                    //s->increase();
                                    s->setValue(s->getValue()+(s->max - s->min)*0.002);
                                }
                                else
                                {
                                    ofxSimpleGuiSliderInt *s = (ofxSimpleGuiSliderInt *) m_gui.getPages()[i]->getControls()[j];
                                    //s->increase();
                                    s->setValue(s->getValue()+1);
                                }
                           }
                        }
                    }
                }
	        }
	    }
	}
}

#endif
