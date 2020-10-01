#include "erasedrawer.h"

EraseDrawer::EraseDrawer(const shape_object &object, QPainter &p,QPainter& pt, QObject *parent)
    :Drawer(parent)
{
    if(object.egeneral){
        int eraseSize=object.erasesize;
        for(auto && x:object.ps){
            QRect r(x.x()-eraseSize/2,x.y()-eraseSize/2,eraseSize,eraseSize);
            p.eraseRect(r);
            auto old=pt.compositionMode();
            pt.setCompositionMode(QPainter::CompositionMode_Clear);
            pt.eraseRect(r);
            pt.setCompositionMode(old);
        }
    }else{
        p.eraseRect(object.rs[0]);
        auto old=pt.compositionMode();
        pt.setCompositionMode(QPainter::CompositionMode_Clear);
        pt.eraseRect(object.rs[0]);
        pt.setCompositionMode(old);
    }
}
