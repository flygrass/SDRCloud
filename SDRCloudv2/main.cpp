#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
static char logFileName[] = "E:/workspace/Qt/SDRCloud/applogs/applog.txt";
//static FILE *logFile = fopen(logFileName, "w");
//// ע��һ����־���������������Ŀ���
//void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
//{
//	QByteArray localMsg = msg.toLocal8Bit();
//	switch (type) {
//	case QtDebugMsg:
//		fprintf(stderr, "Debug: %s(%u, %s)\n", localMsg.constData()/*, context.file*/, context.line, context.function);
//		break;
//	case QtInfoMsg:
//		fprintf(logFile, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//		break;
//	case QtWarningMsg:
//		fprintf(logFile, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//		break;
//	case QtCriticalMsg:
//		fprintf(logFile, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//		break;
//	case QtFatalMsg:
//		fprintf(logFile, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//		abort();
//	}
//}
int main(int argc, char *argv[]){
	/*qInstallMessageHandler(myMessageOutput);*/
	//qDebug() << "start";
	QApplication a(argc, argv);
	MainWindow w;	// ��Ҫ��Ϊ���ڳ������ʱ�ر���־�ļ�
	w.show();
	return a.exec();
}
