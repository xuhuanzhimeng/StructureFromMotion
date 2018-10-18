#include "SFM.h"
#include "qfiledialog.h"
#include <iostream>
#include <string>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qlistwidget.h>
#include <windows.h> 




QString getTime();
using namespace std;

SFM::SFM(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	e = new InputPathWindow();

	ui.label_picture->setFixedSize(960,550);
	ui.tabWidget_picture->clear();
	//sparse
	connect(ui.action_InputPath_3, SIGNAL(triggered()), this, SLOT(InputPath()));//�˵�������Ӵ�������
	connect(ui.action_InitImage, SIGNAL(triggered()), this, SLOT(InitImage()));//��ʼ��ͼ���б�
	connect(ui.action_ComputeFeatures, SIGNAL(triggered()), this, SLOT(ComputeFeatures()));//����������
	connect(ui.action_ComputeMatches, SIGNAL(triggered()), this, SLOT(ComputeMatches()));//ƥ��������
	connect(ui.action_IncrementalSfM, SIGNAL(triggered()), this, SLOT(IncrementalSfM()));//�����ؽ�
	connect(ui.action_ComputeDataColor, SIGNAL(triggered()), this, SLOT(ComputeDataColor()));//�ṹ��ɫ
	connect(ui.action_OpenMVG2openMVS, SIGNAL(triggered()), this, SLOT(OpenMVG2openMVS()));//�ļ�ת��
	connect(ui.action_Sparse, SIGNAL(triggered()), this, SLOT(Sparse()));//����ϡ���ؽ�
	//dense
	connect(ui.action_DensifyPointCloud, SIGNAL(triggered()), this, SLOT(DensifyPointCloud()));//���ܵ���
	connect(ui.action_ReconstructMesh, SIGNAL(triggered()), this, SLOT(ReconstructMesh()));//�ؽ�����
	connect(ui.action_RefineMesh, SIGNAL(triggered()), this, SLOT(RefineMesh()));//ϸ������
	connect(ui.action_TextureMesh, SIGNAL(triggered()), this, SLOT(TextureMesh()));//���������
	connect(ui.action_Dense, SIGNAL(triggered()), this, SLOT(Dense()));//���������ؽ�


	connect(ui.action_ImportCloud, SIGNAL(triggered()), this, SLOT(ImportCloud()));//����ply����
	connect(e, SIGNAL(sendData(QString, QString)), this, SLOT(receiveData(QString, QString)));//�����źźͽ��ܲۺ�������

	connect(ui.action_ClearList, SIGNAL(triggered()), this, SLOT(clearList()));//�������б�

	ui.tabWidget_picture->hide();
	ui.listWidget->setGeometry(0, 550, 1140, 170);
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------�����ѿ�ʼ����----------") + input_dir);
}


void SFM::showPicture(int row)
{
	QListWidget *list = qobject_cast<QListWidget*>(sender());
	QString path = list->windowIconText() + list->item(row)->text();
	QPixmap pixmap(path);
	QPixmap result = pixmap.scaled(960, 540, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.label_picture->setPixmap(result);

	ui.label_picture->show();

}
QStringList getFileNames(QString &path)
{
	QDir dir(path);
	QStringList nameFilters;
	nameFilters << "*.jpg" << "*.svg";
	QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
	return files;
}


void SFM::clearList()
{
	ui.listWidget->clear();
}

void SFM::InputPath()
{
	//��ʼ������
	e->show();
}
void SFM::InitImage()
{
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------��ʼ��ͼƬ�б�ʼ----------") );


	std::string command = sparse_bin_dir.toStdString()+"openMVG_main_SfMInit_ImageListing.exe -i " + input_dir.toStdString()
		+ " -d " + camera_file_params.toStdString() + " -o " + matches_dir.toStdString() + " -f 5030.4";
	system(command.c_str());

	ui.tabWidget_picture->show();
	QListWidget *listInitImage = new QListWidget();
	connect(listInitImage, SIGNAL(currentRowChanged(int)), this, SLOT(showPicture(int)));  //�б�������ʾͼƬ����
	QStringList filenamelist = getFileNames(input_dir);
	QString tmp;
	for (int i = 0; i<filenamelist.size(); i++)
		listInitImage->addItem( filenamelist.at(i));
	listInitImage->setWindowIconText(input_dir + "/");

	ui.tabWidget_picture->addTab(listInitImage, QString::fromLocal8Bit("��ʼͼƬ"));


	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------��ʼ��ͼƬ�б����----------"));
}
void SFM::ComputeFeatures()
{
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------���������㿪ʼ----------"));


	std::string command = sparse_bin_dir.toStdString() +"openMVG_main_ComputeFeatures.exe -i " + matches_dir.toStdString() + "/sfm_data.json"
		+ " -o " + matches_dir.toStdString();
	system(command.c_str());
	command = sparse_bin_dir.toStdString() + "openMVG_main_exportKeypoints.exe -i " + matches_dir.toStdString() + "/sfm_data.json"
		+ " -d " + matches_dir.toStdString() + " -o " + matches_dir.toStdString() + "/exportFeatures";
	system(command.c_str());



	QListWidget *listComputeFeatures = new QListWidget();
	connect(listComputeFeatures, SIGNAL(currentRowChanged(int)), this, SLOT(showPicture(int)));  //�б�������ʾͼƬ����
	QStringList filenamelist = getFileNames(QString(matches_dir+ "/exportFeatures"));
	QString tmp;
	for(int i=0;i<filenamelist.size();i++)
		listComputeFeatures->addItem(filenamelist.at(i));
	listComputeFeatures->setWindowIconText(matches_dir + "/exportFeatures/");
	ui.tabWidget_picture->addTab(listComputeFeatures, QString::fromLocal8Bit("ͼƬ����"));

	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------�������������----------"));
}
void SFM::ComputeMatches()
{
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------ƥ�������㿪ʼ----------"));

	std::string command = sparse_bin_dir.toStdString()+"openMVG_main_ComputeMatches.exe -i " + matches_dir.toStdString() + "/sfm_data.json"
		+ " -o " + matches_dir.toStdString();
	system(command.c_str());
	command = sparse_bin_dir.toStdString() + "openMVG_main_exportMatches.exe -i " + matches_dir.toStdString() + "/sfm_data.json"
		+ " -o " + matches_dir.toStdString() + "/exportMatches" + " -d " + matches_dir.toStdString() + " -m " + matches_dir.toStdString() + "/matches.putative.bin" ;
	system(command.c_str());



	QListWidget *listComputeMatches = new QListWidget();
	connect(listComputeMatches, SIGNAL(currentRowChanged(int)), this, SLOT(showPicture(int)));  //�б�������ʾͼƬ����
	QStringList filenamelist = getFileNames(QString(matches_dir + "/exportMatches"));
	QString tmp;
	for (int i = 0; i<filenamelist.size(); i++)
		listComputeMatches->addItem(filenamelist.at(i));
	listComputeMatches->setWindowIconText(matches_dir + "/exportMatches/");
	ui.tabWidget_picture->addTab(listComputeMatches, QString::fromLocal8Bit("����ƥ��"));

	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------ƥ�����������----------"));
}
void SFM::IncrementalSfM()
{
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------����SFM��ʼ----------"));


	std::string command = sparse_bin_dir.toStdString() +"openMVG_main_IncrementalSfM.exe -i " + matches_dir.toStdString() + "/sfm_data.json"
		+ " -m " + matches_dir.toStdString() + " -o " + reconstruction_incremental_dir.toStdString();
	system(command.c_str());



	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------����SFM���----------"));

	command = "D:/MeshLab/meshlab.exe " + reconstruction_incremental_dir.toStdString() + "/cloud_and_poses.ply";
	system(command.c_str());
}

void SFM::ComputeDataColor()
{
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------�ṹ��ɫ��ʼ----------"));


	std::string command = sparse_bin_dir.toStdString() + "openMVG_main_ComputeSfM_DataColor.exe -i " + reconstruction_incremental_dir.toStdString() + "/sfm_data.bin"
		+ " -o " + reconstruction_incremental_dir.toStdString()+ "/colorized.ply";
	system(command.c_str());


	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------�ṹ��ɫ���----------"));

	command = "D:/MeshLab/meshlab.exe " + reconstruction_incremental_dir.toStdString() + "/colorized.ply";
	system(command.c_str());
}

void SFM::OpenMVG2openMVS()
{
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------ת���ļ����Ϳ�ʼ----------"));


	std::string command = sparse_bin_dir.toStdString() + "openMVG_main_openMVG2openMVS.exe -i " + reconstruction_incremental_dir.toStdString() + "/sfm_data.bin"
		+ " -o " + output_dir.toStdString() + "/scene.mvs";
	system(command.c_str());

	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------ת���ļ��������----------"));
}
void SFM::Sparse()
{
	emit ui.action_InitImage->triggered();
	emit ui.action_ComputeFeatures->triggered();
	emit ui.action_ComputeMatches->triggered();
	emit ui.action_IncrementalSfM->triggered();
	emit ui.action_ComputeDataColor->triggered();
	emit ui.action_OpenMVG2openMVS->triggered();
}




void SFM::DensifyPointCloud()
{
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------���Ƴ��ܻ���ʼ----------"));

	std::string command = dense_bin_dir.toStdString() + "DensifyPointCloud.exe " + output_dir.toStdString() + "/scene.mvs";
	system(command.c_str());

	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------���Ƴ��ܻ�����----------"));

	command = "D:/MeshLab/meshlab.exe " + output_dir.toStdString() + "/scene_dense.ply";
	system(command.c_str());
}
void SFM::ReconstructMesh()
{
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------�����ؽ����----------"));

	std::string command = dense_bin_dir.toStdString() + "ReconstructMesh.exe " + output_dir.toStdString() + "/scene_dense.mvs";
	system(command.c_str());

	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------�����ؽ����----------"));

	command = "D:/MeshLab/meshlab.exe " + output_dir.toStdString() + "/scene_dense_mesh.ply";
	system(command.c_str());
}
void SFM::RefineMesh()
{
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------����ϸ����ʼ----------"));

	std::string command = dense_bin_dir.toStdString() + "RefineMesh.exe " + output_dir.toStdString() + "/scene_dense_mesh.mvs";
	system(command.c_str());

	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------����ϸ�����----------"));

	command = "D:/MeshLab/meshlab.exe " + output_dir.toStdString() + "/scene_dense_mesh_refine.ply";
	system(command.c_str());
}
void SFM::TextureMesh()
{
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------����ӳ�����----------"));

	std::string command = dense_bin_dir.toStdString() + "TextureMesh.exe " + output_dir.toStdString() + "/scene_dense_mesh_refine.mvs";
	system(command.c_str());

	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("----------����ӳ�����----------"));

	command = "D:/MeshLab/meshlab.exe " + output_dir.toStdString() + "/scene_dense_mesh_refine_texture.ply";
	system(command.c_str());
}
void SFM::Dense()
{
	emit ui.action_DensifyPointCloud->triggered();
	emit ui.action_ReconstructMesh->triggered();
	emit ui.action_RefineMesh->triggered();
	emit ui.action_TextureMesh->triggered();
}



void SFM::ImportCloud()
{
	//�����ļ��Ի�����  
	QFileDialog *fileDialog = new QFileDialog(this);
	//�����ļ��Ի������  
	fileDialog->setWindowTitle(tr("select point cloud path"));
	//����Ĭ���ļ�·��  
	fileDialog->setDirectory(".");
	//����ѡ��Ŀ¼
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	//������ͼģʽ  
	fileDialog->setViewMode(QFileDialog::Detail);
	//��ӡ·��  
	QStringList fileNames;
	if (fileDialog->exec())
		fileNames = fileDialog->selectedFiles();
	if (fileNames.empty())
		QMessageBox(QMessageBox::NoIcon, "Attention!", "No path selected!").exec();
	else
		if(system(("D:/MeshLab/meshlab.exe " + fileNames.at(0).toStdString()).c_str()))
			QMessageBox(QMessageBox::NoIcon, "Attention!", "Path is illegal!").exec();
}

void SFM::receiveData(QString in, QString out)
{
	input_dir = in;
	output_dir = out;
	matches_dir = output_dir + "/matches";
	reconstruction_incremental_dir = output_dir + "/reconstruction_incremental";
	camera_file_params = "E:/sensor_width_camera_database.txt";
	sparse_bin_dir = "E:/buildopenMVG/Windows-AMD64-Release/Release/";
	dense_bin_dir = "E:/openMVS/build/openMVS/bin/vc14/x64/Release/";
	
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("ͼƬ��·��:") + input_dir);
	ui.listWidget->addItem(getTime()+QString::fromLocal8Bit("������·��:") + output_dir);
	ui.tabWidget_picture->clear();


}


QString getTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	return QString("%1-%2-%3 %4:%5:%6	").arg(sys.wYear).arg(sys.wMonth, 2, 12, QChar('0')).arg(sys.wDay, 2, 10, QChar('0')).arg(sys.wHour, 2, 10, QChar('0')).arg(sys.wMinute, 2, 10, QChar('0')).arg(sys.wSecond,2,10,QChar('0'));
}