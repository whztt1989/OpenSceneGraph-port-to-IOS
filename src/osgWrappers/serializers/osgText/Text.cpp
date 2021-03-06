#include <osgText/Text>
#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>

// _font
static bool checkFont( const osgText::Text& text )
{
    return text.getFont()!=NULL;
}

static bool readFont( osgDB::InputStream& is, osgText::Text& text )
{
    std::string fontName; is.readWrappedString( fontName );
    text.setFont( osgText::readFontFile(fontName) );
    return true;
}

static bool writeFont( osgDB::OutputStream& os, const osgText::Text& text )
{
    os.writeWrappedString( text.getFont()->getFileName() );
    os << std::endl;
    return true;
}

// _backdropHorizontalOffset, _backdropVerticalOffset
static bool checkBackdropOffset( const osgText::Text& text )
{
    return true;
}

static bool readBackdropOffset( osgDB::InputStream& is, osgText::Text& text )
{
    float horizontal, vertical; is >> horizontal >> vertical;
    text.setBackdropOffset( horizontal, vertical );
    return true;
}

static bool writeBackdropOffset( osgDB::OutputStream& os, const osgText::Text& text )
{
    os << text.getBackdropHorizontalOffset()
       << text.getBackdropVerticalOffset() << std::endl;
    return true;
}

// _colorGradientTopLeft .. _colorGradientBottomRight
static bool checkColorGradientCorners( const osgText::Text& text )
{
    return true;
}

static bool readColorGradientCorners( osgDB::InputStream& is, osgText::Text& text )
{
    osg::Vec4d lt, lb, rb, rt;
    is >> osgDB::BEGIN_BRACKET;
    is >> osgDB::PROPERTY("TopLeft") >> lt;
    is >> osgDB::PROPERTY("BottomLeft") >> lb;
    is >> osgDB::PROPERTY("BottomRight") >> rb;
    is >> osgDB::PROPERTY("TopRight") >> rt;
    is >> osgDB::END_BRACKET;
    text.setColorGradientCorners( lt, lb, rb, rt );
    return true;
}

static bool writeColorGradientCorners( osgDB::OutputStream& os, const osgText::Text& text )
{
    os << osgDB::BEGIN_BRACKET << std::endl;
    os << osgDB::PROPERTY("TopLeft") << osg::Vec4d(text.getColorGradientTopLeft()) << std::endl;
    os << osgDB::PROPERTY("BottomLeft") << osg::Vec4d(text.getColorGradientBottomLeft()) << std::endl;
    os << osgDB::PROPERTY("BottomRight") << osg::Vec4d(text.getColorGradientBottomRight()) << std::endl;
    os << osgDB::PROPERTY("TopRight") << osg::Vec4d(text.getColorGradientTopRight()) << std::endl;
    os << osgDB::END_BRACKET << std::endl;
    return true;
}

REGISTER_OBJECT_WRAPPER( osgText_Text,
                         new osgText::Text,
                         osgText::Text,
                         "osg::Object osg::Drawable osgText::TextBase osgText::Text" )
{
    ADD_USER_SERIALIZER( Font );  // _font
    ADD_VEC4_SERIALIZER( Color, osg::Vec4() );  // _color
    
    BEGIN_ENUM_SERIALIZER( BackdropType, NONE );
        ADD_ENUM_VALUE( DROP_SHADOW_BOTTOM_RIGHT );
        ADD_ENUM_VALUE( DROP_SHADOW_CENTER_RIGHT );
        ADD_ENUM_VALUE( DROP_SHADOW_TOP_RIGHT );
        ADD_ENUM_VALUE( DROP_SHADOW_BOTTOM_CENTER );
        ADD_ENUM_VALUE( DROP_SHADOW_TOP_CENTER );
        ADD_ENUM_VALUE( DROP_SHADOW_BOTTOM_LEFT );
        ADD_ENUM_VALUE( DROP_SHADOW_CENTER_LEFT );
        ADD_ENUM_VALUE( DROP_SHADOW_TOP_LEFT );
        ADD_ENUM_VALUE( OUTLINE );
        ADD_ENUM_VALUE( NONE );
    END_ENUM_SERIALIZER();  // _backdropType
    
    BEGIN_ENUM_SERIALIZER( BackdropImplementation, DEPTH_RANGE );
        ADD_ENUM_VALUE( POLYGON_OFFSET );
        ADD_ENUM_VALUE( NO_DEPTH_BUFFER );
        ADD_ENUM_VALUE( DEPTH_RANGE );
        ADD_ENUM_VALUE( STENCIL_BUFFER );
    END_ENUM_SERIALIZER();  // _backdropImplementation
    
    ADD_USER_SERIALIZER( BackdropOffset );  // _backdropHorizontalOffset, _backdropVerticalOffset
    ADD_VEC4_SERIALIZER( BackdropColor, osg::Vec4() );  // _backdropColor
    
    BEGIN_ENUM_SERIALIZER( ColorGradientMode, SOLID );
        ADD_ENUM_VALUE( SOLID );
        ADD_ENUM_VALUE( PER_CHARACTER );
        ADD_ENUM_VALUE( OVERALL );
    END_ENUM_SERIALIZER();  // _colorGradientMode
    
    ADD_USER_SERIALIZER( ColorGradientCorners );  // _colorGradientTopLeft .. _colorGradientBottomRight
}
