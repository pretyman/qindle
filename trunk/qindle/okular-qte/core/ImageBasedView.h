/* 功能：显示图像模型的视图
 * Copyright (C) 2010 Li Miao <lm3783@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */


#ifndef ImageBasedView_h
#define ImageBasedView_h

#include <QLabel>
#include <QPainter>

namespace okular{
class ImageBasedView : public QLabel {
    Q_OBJECT
public:
    ImageBasedView(QWidget *p);
    int loadImage(QImage image);
//    int loadText(QString text);
    //渲染缓冲，为完整图像大小。
    QSize backSize;
    void setZoomFactor(qreal factor);
    void updateSize();
    Qt::AspectRatioMode scalemode;
public slots:
    void setDisplayType(int newtype);
protected:
    virtual void keyPressEvent ( QKeyEvent * ev );
private:
    //渲染缓冲，大小无意义（通过backSize确定）
    QPixmap backbuffer;
    //可视窗口，大小固定与控件相同，初始座标可变。
    QRect viewerpoint;
    qreal zoomFactor;

    enum Type {Fit, All, Split2H, Split2V, Split4, Split6, prepareTrim, activeTrim};
    enum State {Begin, End, Second, Third, Fourth, Fifth, None, setTrim};
    struct StateMachine {
        Type m_type;
        State m_state;
    };
    StateMachine m_machine;
    
    QPixmap* marginmap;
    QRect* margin;
    QPainter* MarginPainter;
signals:
    void gotoPage(bool gotoNextPage);
    void refresh();
};
}
#endif // ImageBasedView_h
