
#include "customViewer.h"

#include <Inventor/SbBasic.h> 
#include <Inventor/events/SoMouseButtonEvent.h>

#include <QWidget>
#include <QMenu>
#include <QtDebug>


//____________________________________________________________________
CustomExaminerViewer::CustomExaminerViewer(QWidget * parent,
		bool detectorViewButtons,
		const char * name,
		SbBool embed,
		SoQtFullViewer::BuildFlag flag,
		SoQtViewer::Type type)
: SoQtExaminerViewer(parent,(name?name:"CustomExaminerViewer"),embed,flag,type),
  m_popup_menu(0),
  m_popup_antiAliasAction(0)
{
    qDebug() << "Running the 'CustomExaminerViewer' constructor";
}


//____________________________________________________________________
CustomExaminerViewer::~CustomExaminerViewer()
{
#if SOQT_MAJOR_VERSION <= 1 && SOQT_MINOR_VERSION <= 4 && SOQT_MICRO_VERSION <= 1
	//Fix bug in SoQt, in which the menu does not get deleted (reported and fixed upstream after SoQt 1.4.1)
	delete prefmenu;
#endif
	//delete m_d;
	SoQtExaminerViewer::setSceneGraph(0);
}

//____________________________________________________________________
bool CustomExaminerViewer::ensureMenuInit()
{
    qDebug() << "CustomExaminerViewer::ensureMenuInit()";

	if (m_popup_menu)
		return true;
	QWidget * w = getWidget();
	if (!w)
		return false;
	m_popup_menu = new QMenu (w);

	m_popup_antiAliasAction = m_popup_menu->addAction("&Anti aliasing [A]");
    m_popup_antiAliasAction->setCheckable(true);
}


//____________________________________________________________________
void CustomExaminerViewer::showPopupMenu()
{
	qDebug("CustomExaminerViewer: Showing popup menu.");
	if (!ensureMenuInit())
		return;
	//updatePopupMenuStates();

	//Execute
	QAction * selAct = m_popup_menu->exec(QCursor::pos());

    if ( selAct == m_popup_antiAliasAction ) {
        //qDebug("VP1ExaminerViewer::showPopupMenu anti aliasing changed to "+VP1Msg::str(m_d->popup_antiAliasAction->isChecked()));
        setAntiAlias(m_popup_antiAliasAction->isChecked());
        //setAntialiasing(true, 4); 
        grabFocus();//Needed since the GL calls triggered when setting antialiasing makes us loose focus (we obviusly just had it).
        qDebug("Anti-aliasing, done.");
        return;
	}

}

//____________________________________________________________________
void CustomExaminerViewer::setAntiAlias(bool b)
{
	m_isantialias=b;
	b ? setAntialiasing(true,4) : setAntialiasing(false,1);
    qDebug() << "Anti-Aliasing mode set.";
}


//____________________________________________________________________
void CustomExaminerViewer::setAntialiasing(SbBool smoothing, int numPasses)
{
    qDebug() << "CustomExaminerViewer::setAntialiasing() - smoothing:" << smoothing << " numPasses:" << numPasses;

    SoQtExaminerViewer::setAntialiasing(smoothing, numPasses); // Needed for offscreen rendering (i.e. snapshots).

    // --- OLD AA method ---
    //	QGLWidget* qglw = (QGLWidget*)getGLWidget();
    //	QGLFormat fmt = qglw->format();
    //	fmt.setSampleBuffers(smoothing);
    //	fmt.setSamples(numPasses);
    //	qglw->setFormat(fmt);    // note: this is supposedly deprecated..
    //	qglw->makeCurrent();

    //    if(smoothing && numPasses > 1)
    //        glEnable(GL_MULTISAMPLE);
    //    else
    //        glDisable(GL_MULTISAMPLE);

    // --- NEW AA method (26Sep2017) ---
    if (smoothing) {
		qDebug("VP1ExaminerViewer: turning AA on.");

        qDebug("Sep 2017: AA is now done using a new technique so please let hn-atlas-vp1-help@cern.ch know of any problems.");
        //FIXME - remove above messages at some point? EJWM.

        if (getGLRenderAction()->isSmoothing() != smoothing)
                getGLRenderAction()->setSmoothing(smoothing); // --> quite ugly outcome!

        int buffers = 4;
        #if SOQT_MAJOR_VERSION > 1 || (SOQT_MAJOR_VERSION == 1 && SOQT_MINOR_VERSION >= 5)
            if (getSampleBuffers() != buffers)
                setSampleBuffers(buffers);
        #else
            if (buffers > 1)
                qDebug("Multisampling is not supported by SoQT < 1.5, this anti-aliasing mode is disabled");
        #endif
    }
    else {
		qDebug("VP1ExaminerViewer: turning AA off.");
        getGLRenderAction()->setSmoothing(smoothing);
        setSampleBuffers(0);
    }

    m_isantialias=smoothing;
}

//____________________________________________________________________
void CustomExaminerViewer::grabFocus()
{
    qDebug("CustomExaminerViewer::grabFocus()");
	QWidget * w = getGLWidget();
	if (w)
		w->setFocus(Qt::OtherFocusReason);
}


//____________________________________________________________________
SbBool CustomExaminerViewer::processSoEvent(const SoEvent * const evt )
{
//    VP1Msg::messageDebug("VP1ExaminerViewer::processSoEvent()");
//    std::cout << "event type: " << evt->getClassTypeId().getName() << " - " << evt->getTypeId().getName() << std::endl;

	if (evt->getTypeId().isDerivedFrom(SoKeyboardEvent::getClassTypeId())) {
		//We want to add a few shortcuts:
		// "A": View all
		// "P": Perspective camera
		// "O": Orthogonal camera
		// "C": Toggle camera mode
		// "M": Toggle view/selection mode.
		//  If "Q" then we do NOT pass it on to the base class (because that closes the top window!!)

		grabFocus(); //probably redundant since we got the event, but can't hurt.

		if (SO_KEY_PRESS_EVENT(evt,SoKeyboardEvent::V)) {
			viewAll();
			return true;//eat event
		}
        /*
		if (SO_KEY_PRESS_EVENT(evt,SoKeyboardEvent::Q))
			return false;//do not eat event, but do not pass through to base.
		if (SO_KEY_PRESS_EVENT(evt,SoKeyboardEvent::C)) {
			toggleCameraType();
			return true;//eat event
		}
		if (SO_KEY_PRESS_EVENT(evt,SoKeyboardEvent::P)) {
			if (!currentCamIsPerspective() )
				toggleCameraType();
			return true;//eat event
		}
		if (SO_KEY_PRESS_EVENT(evt,SoKeyboardEvent::O)) {
			if (currentCamIsPerspective())
				toggleCameraType();
			return true;//eat event
		}
        */
		if (SO_KEY_PRESS_EVENT(evt,SoKeyboardEvent::M)) {
			setViewing(!isViewing());
			return true;//eat event
		}
		if (SO_KEY_PRESS_EVENT(evt,SoKeyboardEvent::A)) {
			//setAntialiasing(true, 4); // AA ON with Smoothing and 4 passes
			//setAntialiasing(false, 1); // AA OFF
            setAntiAlias(!isAntiAlias());
			grabFocus();//Needed since the GL calls triggered when setting antialiasing makes us loose focus (we obviusly just had it).
			return true;//eat event
		}

		//NB: We could emit keypressed signal when keyboardevent and base
		//    class does not eat event.

	} else if (evt->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
		grabFocus();//Any click grabs focus:
		if (isViewing()) {//In viewing mode we open a popup menu on right clicks:
			const SoMouseButtonEvent * ev_mouse = static_cast<const SoMouseButtonEvent*>(evt);
			if ((ev_mouse->getButton() == SoMouseButtonEvent::BUTTON2/*right click*/)) {
				if (ev_mouse->getState() == SoButtonEvent::DOWN) {
					showPopupMenu();
				}
				return true;//eat all right-clicks in viewing mode.
			}
		}
	}

	return SoQtExaminerViewer::processSoEvent(evt);//pass event through, to the base class
}


