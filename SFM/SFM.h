#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SFM.h"
#include "InputPathWindow.h"
class SFM : public QMainWindow
{
	Q_OBJECT
public slots:

	void showPicture(int);//��ʾͼƬ
	//sparse
	void InputPath();//����·��
	void InitImage();//��ʼ��ͼƬ
	void ComputeFeatures();//����������
	void ComputeMatches();//����ƥ��
	void IncrementalSfM();//�����ؽ�
	void ComputeDataColor();//�ṹ��ɫ
	void OpenMVG2openMVS();//�ļ�ת��
	void Sparse();//ϡ���ؽ�
	//dense
	void DensifyPointCloud();
	void ReconstructMesh();
	void RefineMesh();
	void TextureMesh();
	void Dense();


	void clearList();

	void ImportCloud();
	void receiveData(QString in,QString out);//��������
public:
	SFM(QWidget *parent = Q_NULLPTR);
	InputPathWindow* e;
	QString input_dir, output_dir, matches_dir, 
		 reconstruction_incremental_dir,camera_file_params;
	QString sparse_bin_dir,dense_bin_dir;
private:
	Ui::SFMClass ui;
};
