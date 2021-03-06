#include <osg/Camera>
#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>

#ifndef GL_ACCUM_BUFFER_BIT
    #define GL_ACCUM_BUFFER_BIT 0x00000200
#endif

BEGIN_USER_TABLE( RenderOrder, osg::Camera );
    ADD_USER_VALUE( PRE_RENDER );
    ADD_USER_VALUE( NESTED_RENDER );
    ADD_USER_VALUE( POST_RENDER );
END_USER_TABLE()

USER_READ_FUNC( RenderOrder, readOrderValue )
USER_WRITE_FUNC( RenderOrder, writeOrderValue )

BEGIN_USER_TABLE( BufferComponent, osg::Camera );
    ADD_USER_VALUE( DEPTH_BUFFER );
    ADD_USER_VALUE( STENCIL_BUFFER );
    ADD_USER_VALUE( PACKED_DEPTH_STENCIL_BUFFER );
    ADD_USER_VALUE( COLOR_BUFFER );
    ADD_USER_VALUE( COLOR_BUFFER0 );
    ADD_USER_VALUE( COLOR_BUFFER1 );
    ADD_USER_VALUE( COLOR_BUFFER2 );
    ADD_USER_VALUE( COLOR_BUFFER3 );
    ADD_USER_VALUE( COLOR_BUFFER4 );
    ADD_USER_VALUE( COLOR_BUFFER5 );
    ADD_USER_VALUE( COLOR_BUFFER6 );
    ADD_USER_VALUE( COLOR_BUFFER7 );
    ADD_USER_VALUE( COLOR_BUFFER8 );
    ADD_USER_VALUE( COLOR_BUFFER9 );
    ADD_USER_VALUE( COLOR_BUFFER10 );
    ADD_USER_VALUE( COLOR_BUFFER11 );
    ADD_USER_VALUE( COLOR_BUFFER12 );
    ADD_USER_VALUE( COLOR_BUFFER13 );
    ADD_USER_VALUE( COLOR_BUFFER14 );
    ADD_USER_VALUE( COLOR_BUFFER15 );
END_USER_TABLE()

USER_READ_FUNC( BufferComponent, readBufferComponent )
USER_WRITE_FUNC( BufferComponent, writeBufferComponent )

static osg::Camera::Attachment readBufferAttachment( osgDB::InputStream& is )
{
    osg::Camera::Attachment attachment;
    char type = -1; is >> osgDB::PROPERTY("Type") >> type;
    if ( type==0 )
    {
        is >> osgDB::PROPERTY("InternalFormat") >> attachment._internalFormat;
        return attachment;
    }
    else if ( type==1 )
    {
        is >> osgDB::PROPERTY("Image");
        attachment._image = dynamic_cast<osg::Image*>( is.readObject() );
    }
    else if ( type==2 )
    {
        is >> osgDB::PROPERTY("Texture");
        attachment._texture = dynamic_cast<osg::Texture*>( is.readObject() );
        is >> osgDB::PROPERTY("Level") >> attachment._level;
        is >> osgDB::PROPERTY("Face") >> attachment._face;
        is >> osgDB::PROPERTY("MipMapGeneration") >> attachment._mipMapGeneration;
    }
    else
        return attachment;
    
    is >> osgDB::PROPERTY("MultisampleSamples") >> attachment._multisampleSamples;
    is >> osgDB::PROPERTY("MultisampleColorSamples") >> attachment._multisampleColorSamples;
    return attachment;
}

static void writeBufferAttachment( osgDB::OutputStream& os, const osg::Camera::Attachment& attachment )
{
    os << osgDB::PROPERTY("Type");
    if ( attachment._internalFormat!=GL_NONE )
    {
        os << (char)0 << std::endl;
        os << osgDB::PROPERTY("InternalFormat") << GLENUM(attachment._internalFormat) << std::endl;
        return;
    }
    else if ( attachment._image.valid() )
    {
        os << (char)1 << std::endl;
        os << osgDB::PROPERTY("Image") << attachment._image.get();
    }
    else if ( attachment._texture.valid() )
    {
        os << (char)2 << std::endl;
        os << osgDB::PROPERTY("Texture") << attachment._texture.get();
        os << osgDB::PROPERTY("Level") << attachment._level << std::endl;
        os << osgDB::PROPERTY("Face") << attachment._face << std::endl;
        os << osgDB::PROPERTY("MipMapGeneration") << attachment._mipMapGeneration << std::endl;
    }
    else
    {
        os << (char)-1 << std::endl;
        return;
    }
    
    os << osgDB::PROPERTY("MultisampleSamples") << attachment._multisampleSamples << std::endl;
    os << osgDB::PROPERTY("MultisampleColorSamples") << attachment._multisampleColorSamples << std::endl;
}

// _clearMask
static bool checkClearMask( const osg::Camera& node )
{
    return node.getClearMask()!=(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

static bool readClearMask( osgDB::InputStream& is, osg::Camera& node )
{
    GLbitfield mask = 0;
    if ( is.isBinary() )
    {
        int maskValue; is >> maskValue;
        mask = (GLbitfield)maskValue;
    }
    else
    {
        std::string maskSetString; is >> maskSetString;
        osgDB::StringList maskList; osgDB::split( maskSetString, maskList, '|' );
        for ( unsigned int i=0; i<maskList.size(); ++i )
        {
            const std::string& maskValue = maskList[i];
            if ( maskValue=="COLOR" ) mask |= GL_COLOR_BUFFER_BIT;
            else if ( maskValue=="DEPTH" ) mask |= GL_DEPTH_BUFFER_BIT;
            else if ( maskValue=="ACCUM" ) mask |= GL_ACCUM_BUFFER_BIT;
            else if ( maskValue=="STENCIL" ) mask |= GL_STENCIL_BUFFER_BIT;
        }
    }
    node.setClearMask( mask );
    return true;
}

static bool writeClearMask( osgDB::OutputStream& os, const osg::Camera& node )
{
    GLbitfield mask = node.getClearMask();
    if ( os.isBinary() )
        os << (int)mask;
    else
    {
        std::string maskString;
        if ( mask==GL_COLOR_BUFFER_BIT ) maskString += std::string("COLOR|");
        if ( mask==GL_DEPTH_BUFFER_BIT ) maskString += std::string("DEPTH|");
        if ( mask==GL_ACCUM_BUFFER_BIT ) maskString += std::string("ACCUM|");
        if ( mask==GL_STENCIL_BUFFER_BIT ) maskString += std::string("STENCIL|");
        if ( !maskString.size() ) maskString = std::string("NONE|");
        os << maskString.substr(0, maskString.size()-1) << std::endl;
    }
    return true;
}

// _renderOrder & _renderOrderNum
static bool checkRenderOrder( const osg::Camera& node )
{
    return true;
}

static bool readRenderOrder( osgDB::InputStream& is, osg::Camera& node )
{
    int order = readOrderValue(is);
    int orderNumber = 0; is >> orderNumber;
    node.setRenderOrder( static_cast<osg::Camera::RenderOrder>(order), orderNumber );
    return true;
}

static bool writeRenderOrder( osgDB::OutputStream& os, const osg::Camera& node )
{
    writeOrderValue( os, (int)node.getRenderOrder() );
    os << node.getRenderOrderNum() << std::endl;
    return true;
}

// _bufferAttachmentMap
static bool checkBufferAttachmentMap( const osg::Camera& node )
{
    return node.getBufferAttachmentMap().size()>0;
}

static bool readBufferAttachmentMap( osgDB::InputStream& is, osg::Camera& node )
{
    unsigned int size = is.readSize(); is >> osgDB::BEGIN_BRACKET;
    for ( unsigned int i=0; i<size; ++i )
    {
        is >> osgDB::PROPERTY("Attachment");
        osg::Camera::BufferComponent bufferComponent =
            static_cast<osg::Camera::BufferComponent>( readBufferComponent(is) );
        is >> osgDB::BEGIN_BRACKET;
        osg::Camera::Attachment attachment = readBufferAttachment(is);
        is >> osgDB::END_BRACKET;
        
        if ( attachment._internalFormat!=GL_NONE )
        {
            node.attach( bufferComponent, attachment._internalFormat );
        }
        else if ( attachment._image.valid() )
        {
            node.attach( bufferComponent, attachment._image.get(),
                         attachment._multisampleSamples, attachment._multisampleColorSamples );
        }
        else if ( attachment._texture.valid() )
        {
            node.attach( bufferComponent, attachment._texture.get(),
                         attachment._level, attachment._face, attachment._mipMapGeneration,
                         attachment._multisampleSamples, attachment._multisampleColorSamples );
        }
    }
    is >> osgDB::END_BRACKET;
    return true;
}

static bool writeBufferAttachmentMap( osgDB::OutputStream& os, const osg::Camera& node )
{
    const osg::Camera::BufferAttachmentMap& map = node.getBufferAttachmentMap();
    os.writeSize(map.size()); os<< osgDB::BEGIN_BRACKET << std::endl;
    for ( osg::Camera::BufferAttachmentMap::const_iterator itr=map.begin();
          itr!=map.end(); ++itr )
    {
        os << osgDB::PROPERTY("Attachment"); writeBufferComponent( os, itr->first );
        os << osgDB::BEGIN_BRACKET << std::endl;
        writeBufferAttachment( os, itr->second );
        os << osgDB::END_BRACKET << std::endl;
    }
    os << osgDB::END_BRACKET << std::endl;
    return true;
}

REGISTER_OBJECT_WRAPPER( Camera,
                         new osg::Camera,
                         osg::Camera,
                         "osg::Object osg::Node osg::Group osg::Transform osg::Camera" )
{
    ADD_BOOL_SERIALIZER( AllowEventFocus, true );  // _allowEventFocus
    ADD_USER_SERIALIZER( ClearMask );  // _clearMask
    ADD_VEC4_SERIALIZER( ClearColor, osg::Vec4() );  // _clearColor
    ADD_VEC4_SERIALIZER( ClearAccum, osg::Vec4() );  // _clearAccum
    ADD_DOUBLE_SERIALIZER( ClearDepth, 1.0 );  // _clearDepth
    ADD_INT_SERIALIZER( ClearStencil, 0 );  // _clearStencil
    ADD_OBJECT_SERIALIZER( ColorMask, osg::ColorMask, NULL );  // _colorMask
    ADD_OBJECT_SERIALIZER( Viewport, osg::Viewport, NULL );  // _viewport
    
    BEGIN_ENUM_SERIALIZER( TransformOrder, PRE_MULTIPLY );
        ADD_ENUM_VALUE( PRE_MULTIPLY );
        ADD_ENUM_VALUE( POST_MULTIPLY );
    END_ENUM_SERIALIZER();  // _transformOrder
    
    BEGIN_ENUM_SERIALIZER( ProjectionResizePolicy, HORIZONTAL );
        ADD_ENUM_VALUE( FIXED );
        ADD_ENUM_VALUE( HORIZONTAL );
        ADD_ENUM_VALUE( VERTICAL );
    END_ENUM_SERIALIZER();  // _projectionResizePolicy
    
    ADD_MATRIXD_SERIALIZER( ProjectionMatrix, osg::Matrixd() );  // _projectionMatrix
    ADD_MATRIXD_SERIALIZER( ViewMatrix, osg::Matrixd() );  // _viewMatrix
    ADD_USER_SERIALIZER( RenderOrder );  // _renderOrder & _renderOrderNum
    ADD_GLENUM_SERIALIZER( DrawBuffer, GLenum, GL_NONE );  // _drawBuffer
    ADD_GLENUM_SERIALIZER( ReadBuffer, GLenum, GL_NONE );  // _readBuffer
    
    BEGIN_ENUM_SERIALIZER( RenderTargetImplementation, FRAME_BUFFER );
        ADD_ENUM_VALUE( FRAME_BUFFER_OBJECT );
        ADD_ENUM_VALUE( PIXEL_BUFFER_RTT );
        ADD_ENUM_VALUE( PIXEL_BUFFER );
        ADD_ENUM_VALUE( FRAME_BUFFER );
        ADD_ENUM_VALUE( SEPERATE_WINDOW );
    END_ENUM_SERIALIZER();  // _renderTargetImplementation
    
    ADD_USER_SERIALIZER( BufferAttachmentMap );  // _bufferAttachmentMap
    ADD_OBJECT_SERIALIZER( InitialDrawCallback, osg::Camera::DrawCallback, NULL );  // _initialDrawCallback
    ADD_OBJECT_SERIALIZER( PreDrawCallback, osg::Camera::DrawCallback, NULL );  // _preDrawCallback
    ADD_OBJECT_SERIALIZER( PostDrawCallback, osg::Camera::DrawCallback, NULL );  // _postDrawCallback
    ADD_OBJECT_SERIALIZER( FinalDrawCallback, osg::Camera::DrawCallback, NULL );  // _finalDrawCallback
}
