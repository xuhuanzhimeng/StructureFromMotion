#include "InputPathWindow.h"
#include "qfiledialog.h"
#include "qmessagebox.h"


InputPathWindow::InputPathWindow(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowModality(Qt::ApplicationModal);
	this->setFixedSize(500, 100);
	this->setWindowTitle(QString::fromLocal8Bit("ѡ��·��"));
	//��ʼ���ؼ�
	label1 = new QLabel(QString::fromLocal8Bit("ͼƬ��·��"), this);
	edit1 = new QLineEdit(this);
	edit1->setFixedHeight(20);
	btn1 = new QPushButton("...", this);
	label2 = new QLabel(QString::fromLocal8Bit("������·��"), this);
	edit2 = new QLineEdit(this);
	edit2->setFixedHeight(20);
	btn2 = new QPushButton("...", this);
	btnok = new QPushButton(QString::fromLocal8Bit("ȷ��"), this);
	QGridLayout *mainLayout = new QGridLayout(this);
	mainLayout->addWidget(label1, 0, 0);
	mainLayout->addWidget(label2, 1, 0);
	mainLayout->addWidget(edit1, 0, 1);
	mainLayout->addWidget(edit2, 1, 1);
	mainLayout->addWidget(btn1, 0, 2);
	mainLayout->addWidget(btn2, 1, 2);
	mainLayout->addWidget(btnok, 2, 1, Qt::AlignHCenter);
	//mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	this->setLayout(mainLayout);
	//�źźͲۺ�������

	connect(btn1, SIGNAL(clicked()), this, SLOT(chooseInputPath()));
	connect(btn2, SIGNAL(clicked()), this, SLOT(chooseOutputPath()));
	connect(btnok,SIGNAL(clicked()), this, SLOT(clickOK()));

	connect(edit1, SIGNAL(textChanged(const QString&)), this, SLOT(changeInputPath()));
	connect(edit2, SIGNAL(textChanged(const QString&)), this, SLOT(changeOutputPath()));
	edit1->setText("F:/ReconstructionDataSet/Sceauxcastle");
	edit2->setText("F:/123");
}

void InputPathWindow::clickOK()
{
	
	if (path_input.isEmpty() || path_output.isEmpty())
		QMessageBox(QMessageBox::NoIcon, QString::fromLocal8Bit("ע��!"), QString::fromLocal8Bit("·���Ƿ�!")).exec();
	else
	{
		emit sendData(path_input, path_output);//ʹ��emit�ؼ��ʣ���������
		QMessageBox(QMessageBox::NoIcon, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("·��ѡ��ɹ�!")).exec();
		this->reject();
	}
}

void InputPathWindow::changeInputPath()
{
	path_input = edit1->text();
}
void InputPathWindow::changeOutputPath()
{
	path_output = edit2->text();
}
void InputPathWindow::chooseInputPath()
{
	//�����ļ��Ի�����  
	QFileDialog *fileDialog = new QFileDialog(this);
	//�����ļ��Ի������  
	fileDialog->setWindowTitle(QString::fromLocal8Bit("ѡ��ͼƬ��·��"));
	//����Ĭ���ļ�·��  
	fileDialog->setDirectory(".");
	//����ѡ��Ŀ¼
	fileDialog->setFileMode(QFileDialog::Directory);
	//������ͼģʽ  
	fileDialog->setViewMode(QFileDialog::Detail);
	//��ӡ·��  
	QStringList fileNames;
	if (fileDialog->exec())
		fileNames = fileDialog->selectedFiles();
	if (fileNames.empty())
		QMessageBox(QMessageBox::NoIcon, QString::fromLocal8Bit("ע��"), QString::fromLocal8Bit("û��ѡ��·��!")).exec();
	else
		edit1->setText(fileNames.at(0));
}
void InputPathWindow::chooseOutputPath()
{
	//�����ļ��Ի�����  
	QFileDialog *fileDialog = new QFileDialog(this);
	//�����ļ��Ի������  
	fileDialog->setWindowTitle(QString::fromLocal8Bit("ѡ�����·��"));
	//����Ĭ���ļ�·��  
	fileDialog->setDirectory(".");
	//����ѡ��Ŀ¼
	fileDialog->setFileMode(QFileDialog::Directory);
	//������ͼģʽ  
	fileDialog->setViewMode(QFileDialog::Detail);
	//��ӡ·��  
	QStringList fileNames;
	if (fileDialog->exec())
		fileNames = fileDialog->selectedFiles();
	if (fileNames.empty())
		QMessageBox(QMessageBox::NoIcon, QString::fromLocal8Bit("ע��"), QString::fromLocal8Bit("û��ѡ��·��!")).exec();
	else
		edit2->setText(fileNames.at(0));
}
