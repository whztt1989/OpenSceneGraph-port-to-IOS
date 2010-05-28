/**********************************************************************
 *
 *    FILE:            TexEnvCombine.cpp
 *
 *    DESCRIPTION:    Read/Write osg::TexEnvCombine in binary format to disk.
 *
 *    CREATED BY:        Auto generated by iveGenerated
 *                    and later modified by Rune Schmidt Jensen.
 *
 *    HISTORY:        Created 21.3.2003
 *
 *    Copyright 2003 VR-C
 **********************************************************************/

#include "Exception.h"
#include "TexEnvCombine.h"
#include "Object.h"

using namespace ive;

void TexEnvCombine::write(DataOutputStream* out){
    // Write TexEnvCombine's identification.
    out->writeInt(IVETEXENVCOMBINE);
    // If the osg class is inherited by any other class we should also write this to file.
    osg::Object*  obj = dynamic_cast<osg::Object*>(this);
    if(obj){
        ((ive::Object*)(obj))->write(out);
    }
    else
        out_THROW_EXCEPTION("TexEnvCombine::write(): Could not cast this osg::TexEnvCombine to an osg::Object.");
    // Write TexEnvCombine's properties.
    
    // Write combine method
    out->writeInt(getCombine_RGB());
    out->writeInt(getCombine_Alpha());
    // Write source params
    out->writeInt(getSource0_RGB());
    out->writeInt(getSource1_RGB());
    out->writeInt(getSource2_RGB());
    out->writeInt(getSource0_Alpha());
    out->writeInt(getSource1_Alpha());
    out->writeInt(getSource2_Alpha());
    // Write operand params
    out->writeInt(getOperand0_RGB());
    out->writeInt(getOperand1_RGB());
    out->writeInt(getOperand2_RGB());
    out->writeInt(getOperand0_Alpha());
    out->writeInt(getOperand1_Alpha());
    out->writeInt(getOperand2_Alpha());
    // Write scale and constant color
    out->writeFloat(getScale_RGB());
    out->writeFloat(getScale_Alpha());
    out->writeVec4(getConstantColor());

}

void TexEnvCombine::read(DataInputStream* in){
    // Peek on TexEnvCombine's identification.
    int id = in->peekInt();
    if(id == IVETEXENVCOMBINE){
        // Read TexEnvCombine's identification.
        id = in->readInt();
        // If the osg class is inherited by any other class we should also read this from file.
        osg::Object*  obj = dynamic_cast<osg::Object*>(this);
        if(obj){
            ((ive::Object*)(obj))->read(in);
        }
        else
            in_THROW_EXCEPTION("TexEnvCombine::read(): Could not cast this osg::TexEnvCombine to an osg::Object.");
        // Read TexEnvCombine's properties

        // Read combine method
        setCombine_RGB((GLint)in->readInt());
        setCombine_Alpha((GLint)in->readInt());
        // Read combine source
        setSource0_RGB((GLint)in->readInt());
        setSource1_RGB((GLint)in->readInt());
        setSource2_RGB((GLint)in->readInt());
        setSource0_Alpha((GLint)in->readInt());
        setSource1_Alpha((GLint)in->readInt());
        setSource2_Alpha((GLint)in->readInt());
        // read combine params
        setOperand0_RGB((GLint)in->readInt());
        setOperand1_RGB((GLint)in->readInt());
        setOperand2_RGB((GLint)in->readInt());
        setOperand0_Alpha((GLint)in->readInt());
        setOperand1_Alpha((GLint)in->readInt());
        setOperand2_Alpha((GLint)in->readInt());
        // Read scale and constant color
        setScale_RGB(in->readFloat());
        setScale_Alpha(in->readFloat());
        setConstantColor(in->readVec4());

    }
    else{
        in_THROW_EXCEPTION("TexEnvCombine::read(): Expected TexEnvCombine identification.");
    }
}