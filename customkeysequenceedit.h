#ifndef CUSTOMKEYSEQUENCEEDIT_H
#define CUSTOMKEYSEQUENCEEDIT_H

#include <QKeySequenceEdit>

/*
 * Custom QKeySequenceEdit
 * Only allow typing one sequence
 */
class CustomKeySequenceEdit : public QKeySequenceEdit
{
    Q_OBJECT
public:
    explicit CustomKeySequenceEdit(QWidget *parent = 0);
    ~CustomKeySequenceEdit();
protected:
    void keyPressEvent(QKeyEvent* pEvent) override;
};

#endif // CUSTOMKEYSEQUENCEEDIT_H
