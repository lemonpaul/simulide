#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H


#include "component.h"
#include "itemlibrary.h"

class MAINMODULE_EXPORT TextComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY( QString  text  READ getText WRITE setText )
    Q_PROPERTY( bool Fixed_Width READ fixedW WRITE setFixedW DESIGNABLE true USER true )
    Q_PROPERTY( int  Margin      READ margin WRITE setMargin DESIGNABLE true USER true )
    Q_PROPERTY( int  Border      READ border WRITE setBorder DESIGNABLE true USER true )
    
    public:
        TextComponent( QObject* parent, QString type, QString id );
        ~TextComponent();

 static Component* construct( QObject* parent, QString type, QString id );
 static LibraryItem *libraryItem();
 
        int margin() { return m_margin; }
        void setMargin( int margin );
        
        int border() { return m_border; }
        void setBorder( int border ) { m_border = border; }
        
        bool fixedW() { return m_fixedW; }
        void setFixedW( bool fixedW );

        QString getText() { return m_text->toPlainText(); }
        void setText( QString text ) { m_text->document()->setPlainText( text ); }

        void paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget );

    signals:

    public slots:
        void updateGeometry(int, int, int);

    private:
        QGraphicsTextItem* m_text;
        
        int  m_margin;
        int  m_border;
        bool m_fixedW;
};

#endif // TEXTCOMPONENT_H
