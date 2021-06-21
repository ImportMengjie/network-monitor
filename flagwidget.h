#ifndef FLAGWIDGET_H
#define FLAGWIDGET_H

#include <QWidget>

class FlagWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FlagWidget(QWidget *parent = nullptr);
    FlagWidget(int height,QWidget* parent=nullptr, bool useFlag=true);

    void paintEvent(QPaintEvent *event);

    void setFlag(bool flag){
        this->flag = flag;
//        this->paintEvent(nullptr);
        this->repaint();
    }

    bool getFlag(){
        return flag;
    }

private:
    int height_;
    bool flag;
    bool useFlag;

signals:

};

#endif // FLAGWIDGET_H
