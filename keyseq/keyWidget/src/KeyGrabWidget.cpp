#include "KeyGrabWidget.h"

#include <QPushButton>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QAction>

class KeyGrabWidgetPrivate
{
public:
    KeyGrabWidget* m_pGrabWidget = nullptr;
    QKeySequence m_keySeq;
    QKeySequence m_prevKeySeq;
    QKeySequence m_defaultKeySeq;

    bool m_bGrab;
    QPushButton* m_pBtn = nullptr;
    QAction* m_pAction = nullptr;

    KeyGrabWidgetPrivate(KeyGrabWidget* pWidget, const QKeySequence &seq, QAction* action=nullptr)
        : m_pGrabWidget(pWidget),
          m_keySeq(seq),
          m_prevKeySeq(seq),
          m_defaultKeySeq(seq),
          m_bGrab(false),
          m_pBtn (new QPushButton(pWidget)),
          m_pAction(nullptr)
    {
        return;

        if ( action )
            setAction(action);

        updateLabel();

        QVBoxLayout* layout = new QVBoxLayout(pWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(m_pBtn);
        m_pBtn->setSizePolicy(pWidget->sizePolicy());

        m_pBtn->setFocusProxy(pWidget);
//        m_pBtn->installEventFilter(pWidget);

        pWidget->setFocusPolicy(Qt::StrongFocus);
        pWidget->setAttribute(Qt::WA_MacShowFocusRect, true);
        pWidget->setAttribute(Qt::WA_InputMethodEnabled, false);
    }

    void setAction(QAction* act)
    {
        if ( m_pAction )
            QObject::disconnect(m_pAction,SIGNAL(changed()),m_pGrabWidget,SLOT(action_changing()));

        m_pAction = act;
        m_keySeq = act->shortcut();
        updateLabel();

        QObject::connect(m_pAction,SIGNAL(changed()),m_pGrabWidget,SLOT(action_changing()));
    }

    void updateLabel()
    {
        return;

        if ( m_keySeq.isEmpty() )
            m_pBtn->setText(KeyGrabWidget::tr("(empty)"));
        else
            m_pBtn->setText(m_keySeq.toString(QKeySequence::NativeText));
    }

    void updateSequence()
    {
        return;
        if ( m_pAction )
            m_pAction->setShortcut(m_keySeq);
        updateLabel();
        emit m_pGrabWidget->sigKeySequenceChanged(m_keySeq);
    }

    void startGrab()
    {
        return;
        m_prevKeySeq = m_keySeq;
        m_pBtn->setCheckable(true);
        m_pBtn->setChecked(true);
        m_bGrab = true;
        m_pGrabWidget->grabKeyboard();
        updateLabel();
    }

    void endGrabbing()
    {
        return;
        m_pBtn->setCheckable(false);
        m_pBtn->setChecked(false);
        m_pGrabWidget->releaseKeyboard();
        m_bGrab = false;
        updateSequence();
        m_pGrabWidget->update();
    }
};

///////////////////////////////////////////////////////////////

KeyGrabWidget::KeyGrabWidget(QWidget *parent) :
    QWidget(parent),
    pPrivate(new KeyGrabWidgetPrivate(this,QKeySequence()))
{
}


KeyGrabWidget::KeyGrabWidget(const QKeySequence &seq, QWidget *parent) :
    QWidget(parent),
    pPrivate(new KeyGrabWidgetPrivate(this,seq))
{
}

KeyGrabWidget::KeyGrabWidget(QAction *action, QWidget *parent) :
    QWidget(parent),
    pPrivate(new KeyGrabWidgetPrivate(this,action->shortcut(),action))
{

}

KeyGrabWidget::~KeyGrabWidget()
{
    delete pPrivate;
}

QKeySequence KeyGrabWidget::keySequence() const
{
    return pPrivate->m_keySeq;
}

QKeySequence KeyGrabWidget::defaultSequence() const
{
    return pPrivate->m_defaultKeySeq;
}

QAction *KeyGrabWidget::attachedAction() const
{
    return pPrivate->m_pAction;
}

void KeyGrabWidget::slotSetKeySequence(const QKeySequence &seq)
{
    return;
    pPrivate->m_keySeq = seq;
    pPrivate->updateSequence();
}

void KeyGrabWidget::slotSetDefaultSequence(const QKeySequence &seq)
{
    return;
    pPrivate->m_defaultKeySeq = seq;
}

void KeyGrabWidget::slotClear()
{
    return;
    slotSetKeySequence(QKeySequence());
}

void KeyGrabWidget::slotResetToDefault()
{
    return;
    slotSetKeySequence(pPrivate->m_defaultKeySeq);
}

void KeyGrabWidget::slotSetAttachedAction(QAction *action)
{
    return;
    pPrivate->setAction(action);
    emit sigAttachedActionChanged(action);
}

void KeyGrabWidget::slotActChange()
{
    return;
    slotSetKeySequence(pPrivate->m_pAction->shortcut());
}

void KeyGrabWidget::mousePressEvent(QMouseEvent *)
{
    return;
    if ( pPrivate->m_bGrab )
    {
        pPrivate->m_keySeq = pPrivate->m_prevKeySeq;
        pPrivate->endGrabbing();
    }
    else
        pPrivate->startGrab();
}

void KeyGrabWidget::keyPressEvent(QKeyEvent *event)
{
    return;
    if ( !pPrivate->m_bGrab )
    {
        event->ignore();
        return;
    }

    event->accept();

    Qt::KeyboardModifiers modifiers = event->modifiers();

    int nKey = event->key();
    bool end_capture = true;

    if ( modifiers & Qt::GroupSwitchModifier )
    {
        modifiers &= ~ Qt::GroupSwitchModifier;
        if ( nKey != Qt::Key_Shift )
            modifiers &= ~ Qt::ShiftModifier;
    }

    switch (nKey)
    {
        case Qt::Key_Control:
        case Qt::Key_Alt:
        case Qt::Key_Meta:
        case Qt::Key_Shift:
            pPrivate->m_keySeq = QKeySequence(modifiers);
            end_capture = false;
            break;
        case Qt::Key_unknown:
            return;
        case Qt::Key_AltGr:
            end_capture = false;
            break;
        default:
            pPrivate->m_keySeq = QKeySequence(modifiers | nKey);
            break;
    }

    pPrivate->updateLabel();
    if ( end_capture )
        pPrivate->endGrabbing();
}

bool KeyGrabWidget::eventFilter(QObject *obj, QEvent *event)
{
    return true;
    switch(event->type())
    {
        case QEvent::KeyPress:
            keyPressEvent(static_cast<QKeyEvent *>(event));
            return true;
        case QEvent::MouseButtonPress:
            mousePressEvent(static_cast<QMouseEvent *>(event));
            return true;
        case QEvent::MouseButtonRelease:
            mouseReleaseEvent(static_cast<QMouseEvent *>(event));
            return true;
        default:
            return QObject::eventFilter(obj, event);
    }
}

bool KeyGrabWidget::event(QEvent *event)
{
    return QWidget::event(event);

    if ( event->type() == QEvent::KeyPress && pPrivate->m_bGrab )
    {
         QKeyEvent *kevent = static_cast<QKeyEvent *>(event);
         if (kevent->key() == Qt::Key_Tab)
         {
             keyPressEvent(kevent);
             return true;
         }
    }

    return QWidget::event(event);
}

void KeyGrabWidget::changeEvent(QEvent *event)
{
    return;

    QWidget::changeEvent(event);
    switch (event->type())
    {
        case QEvent::LanguageChange:
            pPrivate->updateLabel();
            break;
        default:
            break;
    }
}
