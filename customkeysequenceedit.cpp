#include "customkeysequenceedit.h"
#include <QDebug>

CustomKeySequenceEdit::CustomKeySequenceEdit(QWidget *parent) : QKeySequenceEdit(parent){

}

CustomKeySequenceEdit::~CustomKeySequenceEdit(){

}


void CustomKeySequenceEdit::keyPressEvent(QKeyEvent* pEvent){

    QKeySequenceEdit::keyPressEvent(pEvent);
    if (this->keySequence().count() > 0) {
        QKeySequenceEdit::setKeySequence(this->keySequence());

        emit editingFinished(); // Optinal, depend on if you need the editingFinished signal to be triggered
    }
}
