#pragma once

#include "DataModel\FrameView.h"

#include <QLayout>

#include <QtMath>

#include <QWheelEvent>



FrameView::FrameView()

{

	this->setObjectName("frame");

	setFrameStyle(Sunken | StyledPanel);

	gview = new GView(this);

	gview->setRenderHint(QPainter::Antialiasing, false);

	gview->setDragMode(QGraphicsView::RubberBandDrag);

	gview->setOptimizationFlags(QGraphicsView::DontSavePainterState);

	gview->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

	gview->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	m_cvMat = 0;



	modelScene = new QGraphicsScene(this);

	inputImageItem = new QGraphicsPixmapItem;

	inputImageItem->setZValue(0);

	modelScene->addItem(inputImageItem);

	//setImage("C:\\Users\\Administrator\\Desktop\\TestImage\\board-01.png");


	currentInfo = new QLabel(tr("Welcome"));


	view()->setScene(modelScene);

	setMinimumSize(100, 100);


	int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);

	QSize iconSize(size, size);



	zoomInIcon = new QToolButton(this);

	zoomInIcon->setAutoRepeat(true);

	zoomInIcon->setAutoRepeatInterval(33);

	zoomInIcon->setAutoRepeatDelay(0);

	zoomInIcon->setIcon(QIcon(":/CV_TEAM/icons/zoom_out2.png"));

	zoomInIcon->setIconSize(iconSize);



	zoomOutIcon = new QToolButton(this);

	zoomOutIcon->setAutoRepeat(true);

	zoomOutIcon->setAutoRepeatInterval(33);

	zoomOutIcon->setAutoRepeatDelay(0);

	zoomOutIcon->setIcon(QIcon(":/CV_TEAM/icons/zoom_in1.png"));

	zoomOutIcon->setIconSize(iconSize);

	zoomOutIcon->setWhatsThis(tr("Click to ZoomOut"));



	zoomFitIcon = new QToolButton(this);

	zoomFitIcon->setAutoRepeat(false);

	zoomFitIcon->setCheckable(true);

	zoomFitIcon->setChecked(false);

	zoomFitIcon->setIcon(QIcon(":/CV_TEAM/icons/Search.png"));

	zoomFitIcon->setIconSize(iconSize);



	zoomSlider = new QSlider(Qt::Vertical, this);

	zoomSlider->setMinimum(0);

	zoomSlider->setMaximum(500);

	zoomSlider->setValue(250);

	zoomSlider->setTickPosition(QSlider::TicksLeft);



	selectModeButton = new QToolButton(this);

	selectModeButton->setIcon(QIcon(":/CV_TEAM/icons/drag2.png"));

	selectModeButton->setIconSize(iconSize);

	selectModeButton->setCheckable(true);

	selectModeButton->setChecked(false);



	connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));

	connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));

	connect(zoomFitIcon, SIGNAL(toggled(bool)), this, SLOT(on_adaptToWindow_clicked(bool)));

	connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));

	connect(selectModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));



	selectModeButton->setChecked(true);

	zoomSliderLayout = new QVBoxLayout;

	zoomSliderLayout->addWidget(zoomInIcon);

	zoomSliderLayout->addWidget(zoomSlider);

	zoomSliderLayout->addWidget(zoomOutIcon);

	zoomSliderLayout->addWidget(zoomFitIcon);

	zoomSliderLayout->addWidget(selectModeButton);



	QVBoxLayout *imgLayout = new QVBoxLayout;

	imgLayout->addWidget(currentInfo);

	imgLayout->addWidget(gview);

	leftLayout = new QHBoxLayout;



	leftLayout->addLayout(imgLayout);

	leftLayout->addLayout(zoomSliderLayout);

	setLayout(leftLayout);

}



void FrameView::setupMatrix()

{

	qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) * qreal(0.02));

	QMatrix matrix;

	matrix.scale(scale, scale);

	gview->setMatrix(matrix);

}

void FrameView::zoomIn(int level)

{

	zoomSlider->setValue(zoomSlider->value() + level);

}

void FrameView::zoomOut(int level)

{

	zoomSlider->setValue(zoomSlider->value() - level);

}



void FrameView::on_adaptToWindow_clicked(bool isAdapt)

{

	QMatrix matrix;

	if (isAdapt)

	{

		QSize m_rect = gview->viewport()->size();

		QRectF m_scene_rect = gview->scene()->sceneRect();

		double x_ration = m_rect.width()*1.0f / m_scene_rect.width();

		double y_ration = m_rect.height()*1.0f / m_scene_rect.height();

		if (x_ration < y_ration)

			y_ration = x_ration;

		matrix.scale(y_ration, y_ration);

		resetView();

	}

	else

	{

		matrix.scale(1, 1);

		zoomSlider->setValue(250);

		resetView();

	}

	gview->setMatrix(matrix);

}



void FrameView::resetView()

{

	zoomSlider->setValue(250);

	setupMatrix();

	gview->ensureVisible(QRectF(0, 0, 0, 0));

}



void FrameView::setImage(QString m_fileName)

{

	setImage(cv::imread(m_fileName.toStdString()));

	return;

}



void FrameView::setImage(cv::Mat m_image)

{

	m_cvMat = m_image;

	QImage qImageResult = cvMatToQImage(m_cvMat);

	inputImageItem->setPixmap(QPixmap::fromImage(qImageResult));

	inputImageItem->setPos(0, 0);

	inputImageItem->setZValue(0);

	modelScene->setSceneRect(-10, -10, inputImageItem->pixmap().width() + 20, inputImageItem->pixmap().height() + 20);

	update();

	emit to_imageChanged();

}



void FrameView::adaptToWindow(bool b)

{

	on_adaptToWindow_clicked(b);

}



void FrameView::setTopInfo(QString info)

{

	currentInfo->setText(info);

}



int FrameView::getPixmapWidth()

{

	if (inputImageItem == nullptr) return -1;

	return inputImageItem->pixmap().width();

}

int FrameView::getPixmapHeight()

{

	if (inputImageItem == nullptr) return -1;

	return inputImageItem->pixmap().height();

}





QImage FrameView::cvMatToQImage(const cv::Mat &inMat)

{

	switch (inMat.type())

	{

		// 8-bit, 4 channel

	case CV_8UC4:

	{

		QImage image(inMat.data,

			inMat.cols, inMat.rows,

			static_cast<int>(inMat.step),

			QImage::Format_ARGB32);



		return image;

	}



	// 8-bit, 3 channel

	case CV_8UC3:

	{

		QImage image(inMat.data,

			inMat.cols, inMat.rows,

			static_cast<int>(inMat.step),

			QImage::Format_RGB888);

		return image.rgbSwapped();

	}



	// 8-bit, 1 channel

	case CV_8UC1:

	{

#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)

		QImage image(inMat.data,

			inMat.cols, inMat.rows,

			static_cast<int>(inMat.step),

			QImage::Format_Grayscale8);//Format_Alpha8 and Format_Grayscale8 were added in Qt 5.5

#else

		static QVector<QRgb> sColorTable;



		// only create our color table the first time

		if (sColorTable.isEmpty())

		{

			sColorTable.resize(256);



			for (int i = 0; i < 256; ++i)

			{

				sColorTable[i] = qRgb(i, i, i);

			}

		}



		QImage image(inMat.data,

			inMat.cols, inMat.rows,

			static_cast<int>(inMat.step),

			QImage::Format_Indexed8);



		image.setColorTable(sColorTable);

#endif

		return image;

	}

	default:

		/*qWarning() << "CVS::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();*/

		break;

	}

	return QImage();

}



QPixmap FrameView::cvMatToQPixmap(const cv::Mat & inMat)

{

	return QPixmap::fromImage(cvMatToQImage(inMat));

}



void FrameView::togglePointerMode()

{

	gview->setDragMode(selectModeButton->isChecked()

		? QGraphicsView::ScrollHandDrag//RubberBandDrag

		: QGraphicsView::RubberBandDrag);//ScrollHandDrag

										 //gview->setInteractive(selectModeButton->isChecked());

}



#ifndef QT_NO_WHEELEVENT

void GView::wheelEvent(QWheelEvent *eve)

{

	QPointF cursorpoint = eve->pos();

	QPointF scenePosCur = this->mapToScene(QPoint(cursorpoint.x(), cursorpoint.y()));



	qreal viewWidth = this->viewport()->width();

	qreal viewHeight = this->viewport()->height();

	qreal horzScale = cursorpoint.x() / viewWidth;

	qreal vertScale = cursorpoint.y() / viewHeight;



	int whellDelta = eve->delta();

	if (whellDelta > 0)

	{

		aview->zoomIn(6);

	}

	else

	{

		aview->zoomOut(6);

	}



	QPointF viewPoint = this->matrix().map(scenePosCur);

	this->verticalScrollBar()->setSliderPosition(int(viewPoint.y() - viewHeight*vertScale));

	this->horizontalScrollBar()->setSliderPosition(int(viewPoint.x() - viewWidth*horzScale));



}

void GView::mouseDoubleClickEvent(QMouseEvent *event)

{//

	QGraphicsView::mouseDoubleClickEvent(event);

}



void GView::mouseMoveEvent(QMouseEvent *event)

{

	QPointF cursorPos = mapToScene(event->pos());

	QString msgs = QString("X= %1 , Y= %2 ;").arg(cursorPos.x()).arg(cursorPos.y());

	if (cursorPos.x() >= 0

		&& cursorPos.x() < aview->getMat().cols

		&& cursorPos.y() >= 0

		&& cursorPos.y() < aview->getMat().rows)

	{

		cv::Mat ghost = aview->getMat();

		if (ghost.channels() == 1)

		{

			uchar aaa = ghost.at<uchar>(cvFloor(cursorPos.y()), cvFloor(cursorPos.x()));

			msgs += QString(" Gray= %1").arg(aaa);

		}

		else if (ghost.channels() == 3)

		{

			cv::Vec3b vvv = ghost.at<cv::Vec3b>(cvFloor(cursorPos.y()), cvFloor(cursorPos.x()));

			int mix = 0.114*vvv[0] + 0.587*vvv[1] + 0.299*vvv[2];//

																 //int b = vvv[0], g= vvv[1], r = vvv[2];

																 //msgs += QString(" Gray= %1 ; B%2-G%3-R%4").arg(mix).arg(b).arg(g).arg(r);

			msgs += QString(" Gray= %1 ;").arg(mix);

		}

	}

	aview->setTopInfo(msgs);

	update();

	QGraphicsView::mouseMoveEvent(event);

}

#endif