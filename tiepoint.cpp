// created by Dr. Weiss


#include <QMatrix>
#include <QMessageBox>
#include <QPixmap>
#include <QtDebug>
#include "tiepoint.h"

TiePoints::TiePoints( QImage image, QWidget *parent ):
        QGraphicsView( parent ), img( image ), w( 5 ), h( 5 )
{
    checkImage();
    for ( ; img.height() / static_cast<qreal>( h ) -
            img.width() / static_cast<qreal>( w ) > 10.0 && h < 10; h++ );
    for ( ; img.width() / static_cast<qreal>( w ) -
            img.height() / static_cast<qreal>( h ) > 10.0 && w < 10; w++ );
    initScene();
    initPoints();
    initLines();
}

TiePoints::TiePoints( QImage image, uint width, uint height, QWidget *parent ):
        QGraphicsView( parent ), img( image ), w( width ), h( height )
{
    checkImage();
    initScene();
    initPoints();
    initLines();
}

TiePoints::~TiePoints() {}

void TiePoints::checkImage()
{
    uint r = 0, g = 0, b = 0;
    if ( img.isNull() )
    {
        img = QImage( 200, 200, QImage::Format_ARGB32 );
    }
    for ( int y = 0; y < img.height(); y++ )
    {
        for ( int x = 0; x < img.width(); x++ )
        {
            QRgb px = img.pixel( x, y );
            r += qRed( px );
            g += qGreen( px );
            b += qBlue( px );
        }
    }
    r /= img.height() * img.width();
    g /= img.height() * img.width();
    b /= img.height() * img.width();
    QRgb color = qRgb( r < 128 ? 255 : 0, g < 128 ? 255 : 0, b < 128 ? 255 : 0 );
    pen = QPen( QColor::fromRgb( color ) );
    pen.setWidth( 2 );
}

void TiePoints::initScene()
{
    xinc = img.width();
    yinc = img.height();
    xinc /= w + 1;
    yinc /= h + 1;
    scene = new QGraphicsScene( img.rect(), this );
    setScene( scene );
    pixmap = scene->addPixmap( QPixmap::fromImage( img ) );
    centerOn( pixmap );
    if ( img.width() > img.height() )
    {
        uint width = qMin( img.width(), 600 );
        setMinimumSize( 200, qRound( 200.0 * img.height() / img.width() ) );
        preferred = QSize( width, width * img.height() / img.width() );
    }
    else
    {
        uint height = qMin( img.height(), 600 );
        setMinimumSize( qRound( 200.0 * img.width() / img.height() ), 200 );
        preferred = QSize( height * img.width() / img.height(), height );
    }
}

void TiePoints::initPoints()
{
    points.resize( h );
    for ( uint row = 0; row < h; row++ )
    {
        points[row].resize( w );
        for ( uint col = 0; col < w; col++ )
        {
            points[row][col] = new TiePointItem( this, row, col );
            points[row][col]->setPos( xinc * ( col + 1 ), yinc * ( row + 1 ) );
            connect( points[row][col], SIGNAL( moved( int, int ) ),
                     this, SLOT( movePoint( int, int ) ) );
            scene->addItem( points[row][col] );
        }
    }
}

void TiePoints::initLines()
{
    horiz.resize( h );
    for ( uint row = 0; row < h; row++ )
    {
        qreal y = yinc * ( row + 1 );
        horiz[row].resize( w + 1 );
        for ( uint col = 0; col < w + 1; col++ )
        {
            horiz[row][col] = scene->addLine( xinc * col, y, xinc * ( col + 1 ), y, pen );
            horiz[row][col]->setZValue( 1 );
        }
    }
    vert.resize( w );
    for ( uint col = 0; col < w; col++ )
    {
        qreal x = xinc * ( col + 1 );
        vert[col].resize( h + 1 );
        for ( uint row = 0; row < h + 1; row++ )
        {
            vert[col][row] = scene->addLine( x, yinc * row, x, yinc * ( row + 1 ), pen );
            vert[col][row]->setZValue( 1 );
        }
    }
}

void TiePoints::clear()
{
    for ( uint row = 0; row < h; row++ )
    {
        for ( uint col = 0; col < w; col++ )
        {
            scene->removeItem( points[row][col] );
            delete points[row][col];
        }
    }
    for ( uint row = 0; row < h; row++ )
    {
        for ( uint col = 0; col < w + 1; col++ )
        {
            scene->removeItem( horiz[row][col] );
            delete horiz[row][col];
        }
    }
    for ( uint col = 0; col < w; col++ )
    {
        for ( uint row = 0; row < h + 1; row++ )
        {
            scene->removeItem( vert[col][row] );
            delete vert[col][row];
        }
    }
    points.clear();
    horiz.clear();
    vert.clear();
}

uint TiePoints::Rows() const
{
    return h;
}

uint TiePoints::Cols() const
{
    return w;
}

QSize TiePoints::Size() const
{
    return QSize( w, h );
}

bool TiePoints::SetRows( int height )
{
    return SetSize( w, height );
}

bool TiePoints::SetCols( int width )
{
    return SetSize( width, h );
}

bool TiePoints::SetSize( int width, int height )
{
    if ( width > 0 && height > 0 )
    {
        clear();
        w = width;
        h = height;
        xinc = img.width();
        yinc = img.height();
        xinc /= w + 1;
        yinc /= h + 1;
        initPoints();
        initLines();
        return true;
    }
    else
    {
        return false;
    }
}

bool TiePoints::SetSize( QSize size )
{
    return SetSize( size.width(), size.height() );
}

QPointF TiePoints::Point( int row, int col ) const
{
    if ( row < 0 )
    {
        return QPointF( qMax( qMin( xinc * ( col + 1 ), img.width() - 1. ), 0. ), 0 );
    }
    else if ( row >= static_cast<int>( h ) )
    {
        return QPointF( qMax( qMin( xinc * ( col + 1 ), img.width() - 1. ), 0. ),
                        img.height() - 1 );
    }
    else if ( col < 0 )
    {
        return QPointF( 0, qMax( qMin( yinc * ( row + 1 ), img.height() - 1. ), 0. ) );
    }
    else if ( col >= static_cast<int>( w ) )
    {
        return QPointF( img.width() - 1,
                        qMax( qMin( yinc * ( row + 1 ), img.height() - 1. ), 0. ) );
    }
    else
    {
        return points[row][col]->pos();
    }
}

bool TiePoints::SetPoint( uint row, uint col, qreal x, qreal y )
{
    if ( x >= 0 && x < img.width() - 1 && y >= 0 && y < img.height() - 1 &&
            row < h && col < w )
    {
        points[row][col]->setPos( x, y );
        movePoint( row, col );
        return true;
    }
    else
    {
        return false;
    }
}

void TiePoints::resizeEvent( QResizeEvent *event )
{
    qreal width = event->size().width(), height = event->size().height();
    setMatrix( QMatrix() );
    scale( 2, 2 );
    scale( 0.5, 0.5 );
    if ( img.width() / width > img.height() / height )
    {
        scale( width / img.width(), width / img.width() );
    }
    else
    {
        scale( height / img.height(), height / img.height() );
    }
    event->accept();
}

QSize TiePoints::sizeHint() const
{
    return preferred;
}

void TiePoints::movePoint( int row, int col )
{
    QPointF center = Point( row, col );
    vert[col][row]->setLine( QLineF( Point( row - 1, col ), center ) );
    vert[col][row + 1]->setLine( QLineF( center, Point( row + 1, col ) ) );
    horiz[row][col]->setLine( QLineF( Point( row, col - 1 ), center ) );
    horiz[row][col + 1]->setLine( QLineF( center, Point( row, col + 1 ) ) );
}

const qreal TiePointItem::small = 10, TiePointItem::large = 25;
const qreal TiePointItem::thin = 2, TiePointItem::thick = 5;

TiePointItem::TiePointItem( TiePoints *graph, uint row, uint col ):
        graph( graph ), row( row ), col( col )
{
    setBrush( small );
    radius = small;
    pen = QPen( QColor::fromRgb( 64, 96, 192, 192 ) );
    pen.setWidthF( thin );
    setFlag( ItemIsMovable, true );
    setAcceptHoverEvents( true );
    setZValue( 2 );
}

TiePointItem::~TiePointItem() {}

void TiePointItem::setBrush( qreal val )
{
    QRadialGradient gradient( 0, 0, val );
    gradient.setColorAt( 0, QColor::fromRgb( 64, 96, 192, 255 ) );
    gradient.setColorAt( 1, QColor::fromRgb( 64, 96, 192, 0 ) );
    brush = QBrush( gradient );
}

QRectF TiePointItem::boundingRect() const
{
    return QRectF( -large - thick / 2, -large - thick / 2,
                   2 * large + thick, 2 * large + thick );
}

void TiePointItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *,
                          QWidget * )
{
    painter->setBrush( brush );
    painter->setPen( pen );
    painter->drawEllipse( QPointF( 0, 0 ), radius, radius );
}

QPainterPath TiePointItem::shape() const
{
    QPainterPath shape;
    shape.addEllipse( QPointF( 0, 0 ), large, large );
    return shape;
}

void TiePointItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
    if ( graph->sceneRect().contains( mapToScene( event->pos() ) ) )
    {
        emit moved( row, col );
        QGraphicsItem::mouseMoveEvent( event );
    }
    else
    {
        event->accept();
    }
}

void TiePointItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
    setBrush( large * 1.5 );
    radius = large;
    pen.setWidthF( thick );
    QGraphicsItem::hoverEnterEvent( event );
}

void TiePointItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
    setBrush( small );
    radius = small;
    pen.setWidthF( thin );
    emit moved( row, col );
    QGraphicsItem::hoverLeaveEvent( event );
}
