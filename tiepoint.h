// created by Dr. Weiss


#pragma once

#include <QBrush>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QRectF>
#include <QResizeEvent>
#include <QRgb>
#include <QSize>
#include <QStyleOptionGraphicsItem>
#include <QVector>
#include <QWidget>

class TiePointItem;

class TiePoints: public QGraphicsView
{
        Q_OBJECT;
    public:
        TiePoints( QImage image, QWidget *parent = NULL );
        TiePoints( QImage image, uint width, uint height, QWidget *parent = NULL );
        virtual ~TiePoints();
        uint Rows() const;
        uint Cols() const;
        QSize Size() const;
        QPointF Point( int row, int col ) const;
    public slots:
        bool SetRows( int height );
        bool SetCols( int width );
        bool SetSize( int width, int height );
        bool SetSize( QSize size );
        bool SetPoint( uint row, uint col, qreal x, qreal y );
    protected:
        virtual void resizeEvent( QResizeEvent *event );
        virtual QSize sizeHint() const;
    private:
        void checkImage();
        void initScene();
        void initPoints();
        void initLines();
        void clear();
    private slots:
        void movePoint( int row, int col );
    private:
        QImage img;
        uint w, h;
        qreal xinc, yinc;
        QSize preferred;
        QPen pen;
        QGraphicsScene *scene;
        QGraphicsPixmapItem *pixmap;
        QVector<QVector<TiePointItem *> > points;
        QVector<QVector<QGraphicsLineItem *> > horiz, vert;
};

class TiePointItem: public QObject, public QGraphicsItem
{
        Q_OBJECT;
    public:
        TiePointItem( TiePoints *graph, uint row, uint col );
        virtual ~TiePointItem();
    signals:
        void moved( int row, int col );
    protected:
        virtual QRectF boundingRect() const;
        virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *,
                            QWidget * );
        virtual QPainterPath shape() const;
        virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
        virtual void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
        virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );
    private:
        void setBrush( qreal val );
    private:
        TiePoints *graph;
        uint row, col;
        QBrush brush;
        QPen pen;
        qreal radius;
        static const qreal small, large, thin, thick;
};
