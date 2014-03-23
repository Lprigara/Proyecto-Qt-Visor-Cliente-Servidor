#include "capturebuffer.h"


bool captureBuffer::present(const QVideoFrame &frame)
{
    // A través de este método nos darán el frame para que lo mostremos.
    QVideoFrame frm;
    frm=frame;
    frm.map(QAbstractVideoBuffer::ReadOnly);

    QImage frameAsImage = QImage(frame.bits(), frame.width(), frame.height(), frame.bytesPerLine(),QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
    frameAsImage.copy();
    frm.unmap();

    //Señal que comunica con el slot image para pasarle los frames
    emit signalImage(frameAsImage);
    return true;
}
