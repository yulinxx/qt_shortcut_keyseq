#ifndef _KEYGRAB_WIDGET_H_
#define _KEYGRAB_WIDGET_H_

#include <QWidget>
#include <QKeySequence>


class KeyGrabWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QKeySequence keySequence READ keySequence WRITE slotSetKeySequence NOTIFY sigKeySequenceChanged USER true)
    Q_PROPERTY(QAction* attachedAction READ attachedAction WRITE slotSetAttachedAction NOTIFY sigAttachedActionChanged)
    Q_PROPERTY(QKeySequence defaultSequence READ defaultSequence WRITE slotSetDefaultSequence)

    friend class KeyGrabWidgetPrivate;
    class KeyGrabWidgetPrivate* pPrivate;

public:
    explicit KeyGrabWidget(QWidget *parent = 0);
    explicit KeyGrabWidget(const QKeySequence& seq, QWidget *parent = 0);
    explicit KeyGrabWidget(QAction* action, QWidget *parent = 0);

    ~KeyGrabWidget();

    QKeySequence keySequence() const;
    QKeySequence defaultSequence() const;
    QAction* attachedAction() const;


public slots:
    void slotSetKeySequence ( const QKeySequence& seq );
    void slotSetDefaultSequence ( const QKeySequence& seq );
    void slotClear();
    void slotResetToDefault();

    void slotSetAttachedAction(QAction* action);

private slots:
    void slotActChange();

protected:
    void mousePressEvent(QMouseEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    bool event(QEvent *event) override;
    void changeEvent(QEvent *event) override;

signals:
    void sigKeySequenceChanged(QKeySequence);
    void sigAttachedActionChanged(QAction*);

};

#endif // _KEYGRAB_WIDGET_H_
