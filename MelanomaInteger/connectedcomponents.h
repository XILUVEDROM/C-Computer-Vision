#ifndef CONNECTEDCOMPONENTS_H
#define CONNECTEDCOMPONENTS_H

#include <opencv2/opencv.hpp>

namespace std
{
/*! \ingroup Imaging
 * \brief Специализация шаблона less для cv::Vec<_Tp1, cn>.
 * \details Позволяет сравнивать 2 вектора по рекурентному правилу:
 *  ЕСЛИ lhs[i] != rhs[i] ТО ВЕРНУТЬ lhs[i]<rhs[i]
 *  ИНАЧЕ ПОВТОРИТЬ ДЛЯ i+1
 * Позволяет использовать cv::Vec<_Tp1, cn> как ключ в словарях, деревьях и т.д.
 */
template <typename _Tp1, int cn >
struct less<cv::Vec<_Tp1, cn>>
{
    bool operator()(const cv::Vec<_Tp1, cn>& lhs, const cv::Vec<_Tp1, cn>& rhs ) const
    {
        int lsize = sizeof(lhs) / sizeof(lhs[0]);
        for (int i=0; i < lsize; ++i)
            if (lhs[i] != rhs[i])
                return lhs[i] < rhs[i];
        return false;
    }
};
/*! \ingroup Imaging
 * \brief Специализация шаблона less для cv::Point_<_Tp1>.
 * \details Позволяет сравнивать 2 точки по правилу:
 *  ЕСЛИ lhs.y!=rhs.y ТО ВЕРНУТЬ lhs.y<rhs.y ИНАЧЕ ВЕРНУТЬ lhs.x<rhs.x
 * Позволяет использовать cv::Point_<_Tp1> как ключ в словарях, деревьях и т.д.
 */
template <typename _Tp1>
struct less<cv::Point_<_Tp1>>
{
    bool operator()(const cv::Point_<_Tp1>& lhs, const cv::Point_<_Tp1>& rhs ) const
    {
        if (lhs.y != rhs.y)
            return lhs.y < rhs.y;
        return lhs.x < rhs.x;
    }
};
}

/*! \ingroup Imaging
 * \brief Класс маски сегмента.
 * \details Обеспечивает маскирование интересующего сегмента
 * (получение множества пикселей сегмента на черном фоне). По
 * сути накладывает segmask в точке lefttop, уменьшая выходную
 * матрицу до размеров segmask.
 */
class SegmentMask
{
    cv::Mat1b _segmask;
    cv::Point _lefttop;
public:
    SegmentMask();
    SegmentMask(const cv::Mat1b& segmask, cv::Point lefttop = {0,0});
    cv::Mat operator()(const cv::Mat& img);
};

/*! \ingroup Imaging
 * \brief Класс карты сегментации.
 * \details Является удобной оберткой для одноименной
 * функции opencv. Обеспечивает поиск, хранение, объединение сегментов,
 * генерирования масок SegmentMask, вычисление описывающих прямоугольников и
 * площадей сегментов.
 */
class ConnectedComponents
{
private:
    cv::Mat1i _labels;
    std::map<int, cv::Rect> _rects;
    std::map<int, int> _areas;
    std::set<int> _labels_set;

    int _connectivity;
    int _ccltype;

public:
    ConnectedComponents();
    ConnectedComponents(const cv::Mat1b& img1b, int connectivity = 8, int ccltype = cv::CCL_WU);
    ConnectedComponents(const ConnectedComponents& cc);
    ConnectedComponents(ConnectedComponents&& cc);
    void operator=(const ConnectedComponents& cc);
    void operator=(ConnectedComponents&& cc);

    bool hasLabel(int label) const;
    int labelCount() const;
    cv::Rect boundingRect(int label) const;
    int segmentArea(int label) const;
    SegmentMask fittedMask(int label) const;
    SegmentMask simpleMask(int label) const;
    int label(cv::Point pos) const;
    const std::set<int> &labelsSet() const;
    std::map<int, int> areas(const cv::Rect roi = cv::Rect()) const;

    std::map<int,std::set<cv::Point>> segmentBorder(int label) const;
    //std::map<cv::Vec2i, int> segmentGraphLinkList() const;

    void join(int label1, int label2);
    int joinToEnviroment(int label);
    void combine(int area_threshold);

    cv::Size size() const;
    cv::Mat3b visualize() const;
    void drawBorders(cv::Mat3b &image, int thickness) const;
};


#endif // CONNECTEDCOMPONENTS_H
