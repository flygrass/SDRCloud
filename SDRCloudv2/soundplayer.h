#pragma once
/*
	������������
*/
#ifndef SOUNDPLAYER_H 
#define SOUNDPLAYER_H


#include <QObject>
#include <QAudioOutput>
#include <QThread>

QT_FORWARD_DECLARE_CLASS(PCMIODevice)
QT_FORWARD_DECLARE_CLASS(Demodulator)

class SoundPlayer : public QObject
{
	Q_OBJECT

public:
	SoundPlayer(Demodulator *demod, QObject *parent=nullptr);
	~SoundPlayer();
	friend class UpdateAudioBufWorker;
	void startPlay();
	void stopPlay();
	void destroy();
	void startUpdateAudioBuf();


private:
	void initSoundPlayer();
	void m_updateAudioBuf();	// �˺�����������Ӧ��ֱ�ӵ���
private slots:
	void handleStateChanged(QAudio::State newState);
signals:
	void updateAudioBufSignal(SoundPlayer *player);
private:
	UpdateAudioBufWorker *updateAudioBufWorker;
	QThread updateAudioBufThread;	// ����audio��buffer���߳�
	// ��������
	QAudioFormat format;
	QAudioOutput *m_audio;
	PCMIODevice *m_pcmIODevice;
	Demodulator *m_demod;
};

class UpdateAudioBufWorker :public QObject
{
	Q_OBJECT
public slots:
	void doWork(SoundPlayer *player);
signals:
	void done();
};
#endif // !SOUNDPLAYER_H 
