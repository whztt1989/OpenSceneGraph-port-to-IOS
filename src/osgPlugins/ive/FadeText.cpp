/**********************************************************************
 *
 *    FILE:            FadeText.cpp
 *
 *    DESCRIPTION:    Read/Write osgText::FadeText in binary format to disk.
 *
 *    CREATED BY:        Auto generated by iveGenerator
 *                    and later modified by Rune Schmidt Jensen.
 *
 *    HISTORY:        Created 27.3.2003
 *
 *    Copyright 2003 VR-C
 **********************************************************************/

#include "Exception.h"
#include "FadeText.h"
#include "Text.h"
#include "Drawable.h"
#include "Object.h"

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osg/Notify>

using namespace ive;

void FadeText::write(DataOutputStream* out){
    // Write FadeText's identification.
    out->writeInt(IVEFADETEXT);
    // If the osg class is inherited by any other class we should also write this to file.
    osgText::Text*  text = dynamic_cast<osgText::Text*>(this);
    if(text){
        ((ive::Text*)(text))->write(out);
    }
    else
        out_THROW_EXCEPTION("FadeText::write(): Could not cast this osgText::FadeText to an osgText::Tex.");

    // Write FadeText's properties.
    out->writeFloat(getFadeSpeed());
}

void FadeText::read(DataInputStream* in){
    // Read FadeText's identification.
    int id = in->peekInt();
    if(id == IVEFADETEXT){

        id = in->readInt();
        // If the osg class is inherited by any other class we should also read this from file.
        osgText::Text*  text = dynamic_cast<osgText::Text*>(this);
        if(text){
            ((ive::Text*)(text))->read(in);
        }
        else
            in_THROW_EXCEPTION("FadeText::read(): Could not cast this osgText::FadeText to an osgText::Text.");
        
        setFadeSpeed(in->readFloat());
    }
    else{
        in_THROW_EXCEPTION("FadeText::read(): Expected FadeText identification.");
    }
}
