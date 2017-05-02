#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H


#include "component.h"
#include "itemlibrary.h"

class TextComponent : public Component
{
    Q_OBJECT
    public:
        TextComponent( QObject* parent, QString type, QString id );
        ~TextComponent();

 static Component* construct( QObject* parent, QString type, QString id );
 static LibraryItem *libraryItem();

        void paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget );

    signals:

    public slots:
        void updateGeometry(int, int, int);

    private:
        QGraphicsTextItem* m_text;
};

#endif // TEXTCOMPONENT_H
