#ifndef MTX_MKVTOOLNIX_GUI_CHAPTER_EDITOR_TOOL_H
#define MTX_MKVTOOLNIX_GUI_CHAPTER_EDITOR_TOOL_H

#include "common/common_pch.h"

#include "mkvtoolnix-gui/main_window/tool_base.h"
#include "mkvtoolnix-gui/util/files_drag_drop_handler.h"

class QDragEnterEvent;
class QDropEvent;
class QMenu;

namespace mtx { namespace gui { namespace ChapterEditor {

namespace Ui {
class Tool;
}

class Tab;

class Tool : public ToolBase {
  Q_OBJECT;

protected:
  // UI stuff:
  std::unique_ptr<Ui::Tool> ui;
  QMenu *m_chapterEditorMenu;
  mtx::gui::Util::FilesDragDropHandler m_filesDDHandler;

public:
  explicit Tool(QWidget *parent, QMenu *chapterEditorMenu);
  ~Tool();

  virtual void retranslateUi() override;

public slots:
  virtual void toolShown() override;
  virtual void tabTitleChanged();
  virtual void enableMenuActions();
  virtual void newFile();
  virtual void selectFileToOpen();
  virtual void save();
  virtual void saveAsXml();
  virtual void saveToMatroska();
  virtual bool closeTab(int index);
  virtual void closeCurrentTab();
  virtual void closeSendingTab();
  virtual bool closeAllTabs();
  virtual void reload();
  virtual void filesDropped(QStringList const &fileNames);

protected:
  Tab * appendTab(Tab *tab);

  virtual void openFile(QString const &fileName);
  virtual void setupActions();
  virtual void showChapterEditorsWidget();
  virtual Tab *currentTab();

  virtual void dragEnterEvent(QDragEnterEvent *event) override;
  virtual void dropEvent(QDropEvent *event) override;
};

}}}

#endif // MTX_MKVTOOLNIX_GUI_CHAPTER_EDITOR_TOOL_H
