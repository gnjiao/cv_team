#pragma once

#include "DataModel\InputBox.h"
#include "DataModel\FrameView.h"
#include "BlobOperator.h"
#include "GraphicsItem\PointSetItem.h"
#include <QDialog>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QToolBar>
#include <QFileDialog>
#include <QComboBox>
#include <QTabWidget>
#include <QHeaderView>
#include <QSplitter>
#include <QGroupBox>
#include <QList>
#include <QStringList>
#include <QFileDialog>

class BlobWidget : public QDialog
{
	Q_OBJECT

public:
	BlobWidget(QWidget *parent = Q_NULLPTR);
	~BlobWidget();
private slots:
	void on_action_openFile_triggered();
	void on_action_play_triggered();
	void on_action_edit_toggled(bool);
	void on_action_editCancel_triggered();
	void on_OK_btn_clicked();
	void on_Cancel_btn_clicked();

	void on_image_cb_Changed(int);
	void on_Thresh_sl_Changed(int);
	void on_Thresh_Changed(int);
	void on_Layers_cb_Changed(int);
	void on_Feature_cb_Changed(int);
	void on_Mode_cb_Changed(int);
	void on_Max_sp_Changed(int);
	void on_Min_sp_Changed(int);
	void on_Add_btn_clicked();
	void on_Delete_btn_clicked();
	void on_AddAll_btn_clicked();
	void on_DeleteAll_btn_clicked();
	void on_resultTableWidget_cellClicked(int, int);
	void on_resultTableHeaderDoubleClicked(int);

private:
	void makeToolBar();
	void makeWidget();
	void makeTableWidget();
	void makeMainLayout();

	void makeBaseParamWidget();
	void initWidget();

	void updateResultTableWidget();
	void updateFeatureTableWidget();

	QToolBar *toolBar;

	QTabWidget *TabWidget;

	QWidget *BaseParamWidget;

	QWidget *SetItemWidget;

	QWidget *OutputWidget;

	QLabel *botMsgBox;

	FrameView *fr_view;

	QAction *action_openFile;

	QAction *action_play;

	QAction *action_edit;

	QAction *action_editCancel;


	QPushButton *OK_btn;

	QPushButton *Cancel_btn;

	QFileDialog *openFileDlg;


	InputBox *Image_cb;

	QSlider *Thresh_sl;
	QSpinBox *Thresh_sp;

	QComboBox *Layers_cb;
	QComboBox *Feature_cb;
	QComboBox *Mode_cb;

	QSlider *Max_sl;
	QDoubleSpinBox *Max_sp;

	QSlider *Min_sl;
	QDoubleSpinBox *Min_sp;

	QPushButton *Add_btn;
	QPushButton *Delete_btn;

	QPushButton *AddAll_btn;
	QPushButton *DeleteAll_btn;

	QTableWidget *resultTableWidget;
	QHeaderView *headerView;

	QTableWidget *featureTableWidget;

private:
	void syncOperator(BlobOperator* AA, BlobOperator* BB);
	void updateOutput();
	void updateParameter(bool flag);
	void updateResults(std::vector<SIMPLEBLOB_SPACE::CResult>&m);
	void initialFeature();
	void initialFeatureMode();
	void updateTableWidgetFeatures();
	void updateRowFeatures(int row, SIMPLEBLOB_SPACE::feature mfeature);
	void updateFeatureValue(bool add);
	void updateHorizonHeaders();
	void setAllUes(bool flag);
	void showMoveWindows(bool flag);

	cv::Mat m_currentImage;
	cv::Mat m_thresholdedImage;
	cv::Mat m_featureImage;
	BlobOperator *CalOperator = nullptr;
	BlobOperator *SaveOperator = nullptr;
	PointSetItem *itemOrg;

	std::vector<QString> featureNames;
	QStringList selctfeatureNames;
	std::vector<int> FeaturesIsSlected;
	int sortIndex;

};
