#pragma once
#include "DataModel\BaseWidget.h"
#include "DataModel\InputBox.h"
#include "FindLineOperator.h"
#include "GraphicsItem\findLineItem.h"
#include "GraphicsItem\PointSetItem.h"
#include "GraphicsItem\LineSetItem.h"
#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include <QIntValidator>
#include <QColor>
#include <QColorDialog>
using namespace geo;
class FindLineWidget : public BaseWidget
{
	Q_OBJECT
public:
	FindLineWidget(QWidget *parent = Q_NULLPTR);
	~FindLineWidget();
	void doPlay();
	void doOk();
	void doCancel();
	void doEdit(bool b);
	void doEditSave();
	void setOperator(FindLineOperator *Opra);
	void updateOutput();
	InputBox *getImageComboBox();
	InputBox *getTransComboBox();
private slots:
	void on_image_cb_Changed(int);
	void on_trans_cb_Changed(int);
	void on_polar_cb_Changed(int);
	void on_resultType_cb_Changed(int);

	void on_Cal_Treshold_sl_Changed(int);
	void on_Cal_Width_sl_Changed(int);
	void on_Cal_Height_sl_Changed(int);
	void on_FittingType_cb_Changed(int);
	void on_Itr_Count_sl_Changed(int);
	void on_Caliper_Count_sl_Changed(int);
	void on_Distance_Thresh_sl_Changed(int);
	void on_Delete_Count_sl_Changed(int);

	void on_Cal_Treshold_Changed(int);
	void on_Cal_Width_Changed(int);
	void on_Cal_Height_Changed(int);
	void on_Itr_Count_Changed(int);
	void on_Caliper_Count_Changed(int);
	void on_Distance_Thresh_Changed(int);
	void on_Delete_Count_Changed(int);
	void on_Confirm_btn_Clicked(bool);
	void on_Horizontal_rb_Clicked(bool);
	void on_Vertical_rb_Clicked(bool);
	void on_Degree45_rb_Clicked(bool);
	void on_Degree_45_rb_Clicked(bool);
	void on_FindLineItem_cb_Clicked(bool);
	void on_ResultLine_cb_Clicked(bool);
	void on_OK_Points_cb_Clicked(bool);
	void on_NG_Points_cb_Clicked(bool);
	void on_SelectAll_cb_Clicked(bool);
	void on_FindLineItem_btn_Clicked(bool);
	void on_ResultLine_btn_Clicked(bool);
	void on_OK_Points_btn_Clicked(bool);
	void on_NG_Points_btn_Clicked(bool);
	void on_OK_Points_cob_Changed(int);
	void on_NG_Points_cob_Changed(int);
	void on_FindLineItem_getcolor(QColor);
	void on_ResultLine_getcolor(QColor);
	void on_OK_Points_getcolor(QColor);
	void on_NG_Points_getcolor(QColor);
private:
	void makeBaseParamWidget();
	void makeSetItemWidget();
	void makeOutputWidget();
	void initWidget();
	void initItem();
	void syncOperator(FindLineOperator* AA, FindLineOperator* BB);
	void showImage();
	InputBox *image_cb;
	InputBox *trans_cb;
	QComboBox *polar_cb;
	QComboBox *resultType_cb;
	//Calliper
	QSpinBox *Cal_Treshold_sp;
	QSlider *Cal_Treshold_sl;
	QSpinBox *Cal_Width_sp;
	QSlider *Cal_Width_sl;
	QSpinBox *Cal_Height_sp;
	QSlider *Cal_Height_sl;
	QComboBox *FittingType_cb;
	QComboBox *Hollow_cb;
	QSpinBox *Itr_Count_sp;
	QSlider *Itr_Count_sl;
	QSpinBox *Distance_Thresh_sp;
	QSlider *Distance_Thresh_sl;
	QSpinBox *Delete_Count_sp;
	QSlider *Delete_Count_sl;
	//set Item
	QSpinBox *Caliper_Count_sp;
	QSlider *Caliper_Count_sl;
	QLineEdit *StartPointX_le;
	QLineEdit *StartPointY_le;
	QLineEdit *EndPointX_le;
	QLineEdit *EndPointY_le;
	QPushButton *Confirm_btn;
	QRadioButton *Horizontal_rb;
	QRadioButton *Vertical_rb;
	QRadioButton *Degree45_rb;
	QRadioButton *Degree_45_rb;
	QCheckBox *FindLineItem_cb;
	QPushButton *FindLineItem_btn;
	QCheckBox *ResultLine_cb;
	QPushButton *ResultLine_btn;
	QCheckBox *OK_Points_cb;
	QComboBox *OK_Points_cob;
	QPushButton *OK_Points_btn;
	QCheckBox *NG_Points_cb;
	QComboBox *NG_Points_cob;
	QPushButton *NG_Points_btn;
	QCheckBox *SelectAll_cb;
	QPushButton *SelectAll_btn;
	QLabel *function_lb;
	QLabel *StartPointX_lb;
	QLabel *StartPointY_lb;
	QLabel *MidPointX_lb;
	QLabel *MidPointY_lb;
	QLabel *EndPointX_lb;
	QLabel *EndPointY_lb;
	QLabel *Length_lb;
	QLabel *Degree_lb;
	QLabel *OK_Count_lb;
	QLabel *NG_Count_lb;
	QCheckBox *SetEndPoint_cb;
	QLineEdit *StartPointX_min_le;
	QLineEdit *StartPointX_max_le;
	QCheckBox *StartPointX_cb;
	QLineEdit *StartPointY_min_le;
	QLineEdit *StartPointY_max_le;
	QCheckBox *StartPointY_cb;
	QLineEdit *EndPointX_min_le;
	QLineEdit *EndPointX_max_le;
	QCheckBox *EndPointX_cb;
	QLineEdit *EndPointY_min_le;
	QLineEdit *EndPointY_max_le;
	QCheckBox *EndPointY_cb;
	QColorDialog *m_ColorDialog;
	QColor FindLineItem_color = QColor(255, 0, 0);
	QColor ResultLine_color = QColor(255, 0, 0);
	QColor OKPoints_color = QColor(255, 0, 0);
	QColor NGPoints_color = QColor(255, 0, 0);

private:
	FindLineOperator *m_SaveOperator;
	FindLineOperator *m_CalOperator;
	findLineItem *m_findLineItem;
	LineSetItem *m_ResultLineItem;
	PointSetItem * m_OKPointsItem = nullptr;
	PointSetItem * m_NGPointsItem = nullptr;
	geo::RotRect2D m_refRect;
	vector<geo::RotRect2D> m_refDivideRects;
	geo::AffineTrans m_trans;
};