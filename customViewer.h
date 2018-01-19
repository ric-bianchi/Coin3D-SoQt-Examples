#ifndef CUSTOMEXAMINERVIEWER_H
#define CUSTOMEXAMINERVIEWER_H

//#include "VP1Base/VP1String.h"
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <QObject>

class QPixmap;
class QMenu;
class QAction;

class CustomExaminerViewer : public SoQtExaminerViewer { 
public:   
          CustomExaminerViewer( QWidget * parent = 0, 
                             bool detectorViewButtons = true, 
                             const char * name = 0, 
                             SbBool embed = TRUE, 
                             SoQtFullViewer::BuildFlag flag = BUILD_ALL, 
                             SoQtViewer::Type type = BROWSER);

          virtual ~CustomExaminerViewer();//NB: SoQtExaminerViewer has non-virtual destructor! 
          bool ensureMenuInit();
          void showPopupMenu();
          virtual SbBool processSoEvent(const SoEvent * const event);

          //For consistency, use these instead of setNumPasses:
	      bool isAntiAlias() const {return m_isantialias;};
	      void setAntiAlias(bool);


private:

	QMenu * m_popup_menu;
    QAction* m_popup_antiAliasAction;
    bool m_isantialias;

    void setAntialiasing(SbBool smoothing, int numPasses);
    void grabFocus();
};


#endif


