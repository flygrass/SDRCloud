#include <QImage>

typedef qint8 Gray;
bool histeq(QImage &img)
{
	int *rs = new int[256];
	Gray *T = new Gray[256];
	// ��ʼ��
	for (int i = 0; i < 256; i++) {
		rs[i] = 0;
		T[i] = 0;
	}
	int width = img.width();
	int height = img.height();
	// ͳ��ͼ���еĻҶ�ֵ
	int x = 0;
	int y = 0;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			rs[qGray(img.pixel(x, y))]++;
		}
	}

	// ����Ҷ�ֵӳ���s = T(r)
	int imgSize = width * height;
	for (int i = 0; i < 256; i++) {
		double temp = 0;
		for (int j = 0; j <= i; j++) {
			temp += rs[j];
		}
		T[i] = round(255 * temp / imgSize);
	}

	// ʵ��ӳ��r->s
	int s = 0;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			s = qGray(img.pixel(x, y));
			img.setPixel(x, y, qRgb(T[s], T[s], T[s]));
		}
	}
	// �ͷ��ڴ�
	delete rs;
	rs = nullptr;
	delete T;
	T = nullptr;
	return true;
}