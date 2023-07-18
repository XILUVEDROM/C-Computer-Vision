#include "connectedcomponents.h"
/*!
 * \brief Пустой конструктор
 */
SegmentMask::SegmentMask(){}
/*!
 * \brief Конструктор по маске и верхнему левому углу.
 * \param segmask - маска
 * \param lefttop - точка смещения
 */
SegmentMask::SegmentMask(const cv::Mat1b &segmask, cv::Point lefttop):
    _segmask(segmask), _lefttop(lefttop){}

/*!
 * \brief Наложение маски
 * \param img - оригинальное изображение
 * \return изображение сегмента на черном фоне (размеры = размерам маски)
 */
cv::Mat SegmentMask::operator()(const cv::Mat &img)
{
    cv::Mat ret;
    cv::Rect roi = {_lefttop.x, _lefttop.y, _segmask.cols, _segmask.rows};
    img(roi).copyTo(ret, _segmask);
    return ret;
}

/*!
 * \brief Пустой конструктор
 */
ConnectedComponents::ConnectedComponents():
    _connectivity(-1), _ccltype(-1)
{

}

/*!
 * \brief Конструктор, сегментирующий изображение.
 * \details Обеспечивает поиск одноцветных связных сегментов на img1b (opencv connectedComponentsWithStats),
 * и сохранение информации о них. Сегмент(ы) с яркостью 0 всегда принимается за единый сегмент фона, а его
 * идентификатор = 0.
 * \param img1b - изображение (подготовленное)
 * \param connectivity - связность сегментов (4 или 8 соседей)
 * \param ccltype - тип поиска областей
 * (см. https://docs.opencv.org/3.4.0/d3/dc0/group__imgproc__shape.html#ga5ed7784614678adccb699c70fb841075)
 */
ConnectedComponents::ConnectedComponents(const cv::Mat1b &img1b, int connectivity, int ccltype):
    _labels(cv::Mat1i(img1b.size())), _connectivity(connectivity), _ccltype(ccltype)
{
    using namespace cv;
    Mat1i stats;
    Mat1d centroids;
    connectedComponentsWithStats(img1b, _labels, stats, centroids, _connectivity, _labels.type(), _ccltype);
    for(int label = 0; label < stats.rows; ++label)
    {
        int x, y, w, h;
        const int* ptr = stats[label];
        x = ptr[CC_STAT_LEFT];
        y = ptr[CC_STAT_TOP];
        w = ptr[CC_STAT_WIDTH];
        h = ptr[CC_STAT_HEIGHT];
        _rects[label] = Rect(x, y, w, h);
        _areas[label] = ptr[CC_STAT_AREA];
        _labels_set.insert(label);
    }
}

/*!
 * \brief Копирующий конструктор.
 * \param cc - карта сегментации.
 */
ConnectedComponents::ConnectedComponents(const ConnectedComponents &cc)
{
    _labels = cc._labels;
    _rects = cc._rects;
    _areas = cc._areas;
    _labels_set = cc._labels_set;

    _connectivity = cc._connectivity;
    _ccltype = cc._ccltype;
}

/*!
 * \brief Перемещающий конструктор.
 * \param cc - карта сегментации
 */
ConnectedComponents::ConnectedComponents(ConnectedComponents &&cc)
{
    cv::swap(_labels, cc._labels);
    std::swap(_rects, cc._rects);
    std::swap(_areas, cc._areas);
    std::swap(_labels_set, cc._labels_set);

    _connectivity = cc._connectivity;
    _ccltype = cc._ccltype;
}

/*!
 * \brief Перемещающее присваивание.
 * \param cc - карта сегментации.
 */
void ConnectedComponents::operator=(ConnectedComponents &&cc)
{
    cv::swap(_labels, cc._labels);
    std::swap(_rects, cc._rects);
    std::swap(_areas, cc._areas);
    std::swap(_labels_set, cc._labels_set);

    _connectivity = cc._connectivity;
    _ccltype = cc._ccltype;
}

/*!
 * \brief Копирующее присваивание
 * \param cc - карта сегментации.
 */
void ConnectedComponents::operator=(const ConnectedComponents &cc)
{
    _labels = cc._labels;
    _rects = cc._rects;
    _areas = cc._areas;
    _labels_set = cc._labels_set;

    _connectivity = cc._connectivity;
    _ccltype = cc._ccltype;
}

/*!
 * \brief проверка на наличие метки
 * \param label - идентификатор сегмента (метка)
 * \return true - если сегмент с такой меткой существует.
 */
bool ConnectedComponents::hasLabel(int label) const
{
    return _labels_set.find(label) != _labels_set.end();
}
/*!
 * \brief Число сегментов.
 * \return число сегментов.
 */
int ConnectedComponents::labelCount() const
{
    return int(_labels_set.size());
}

/*!
 * \brief Описывающий прямоугольник.
 * \param label - идентификатор сегмента (метка).
 * \return описывающий сегмент прямоугольник
 */
cv::Rect ConnectedComponents::boundingRect(int label) const
{
    /*if (!hasLabel(label)) std::abort(); */
    return _rects.at(label);
}

/*!
 * \brief Площадь сегмента.
 * \param label - идентификатор сегмента (метка)
 * \return площадь сегмента
 */
int ConnectedComponents::segmentArea(int label) const
{
    /*if (!hasLabel(label)) std::abort(); */
    return _areas.at(label);
}
/*!
 * \brief Подогнанная маска.
 * \details Данная маска имеет размеры охватывающего прямоугольника и
 * смещена в его верхний левый угол. Для такой маски не нужно заботиться о настройке зоны интереса -
 * будучи наложенной на оригинальное изображение она вернет нужный сегмет на черном фоне, без
 * лишних черных краев как в simpleMask.
 * * Пример использования
 * \code
    cv::threshold(src, ret, 0 ,255, THRESH_BINARY_INV | THRESH_OTSU);
    ConnectedComponents cc(ret);
    if (cc.hasLabel(1))
        cv::imshow(cc.fittedMask(1)(src));
 * \endcode
 * \param label - идентификатор сегмента (метка).
 * \return
 */
SegmentMask ConnectedComponents::fittedMask(int label) const
{
    /*if (!hasLabel(label)) std::abort(); */
    using namespace cv;
    Rect bounding_rect = boundingRect(label);
    Mat1i mask = _labels(bounding_rect);
    auto lambda = [&](int& pix, const int*)
        {pix = int(pix == label);};
    mask.forEach(lambda);
    return SegmentMask(mask, bounding_rect.tl());
}

/*!
 * \brief Простая маска.
 * \details При наложении простой маски размер изображения сохранится,
 * оставляя много лишних черных пикселей, в отличае от fittedMask.
 * \param label - идентификатор сегмента (метка).
 * \return
 */
SegmentMask ConnectedComponents::simpleMask(int label) const
{
    using namespace cv;
    Mat1i mask(_labels);
    auto lambda = [&](int& pix, const int*)
        {pix = int(pix == label);};
    mask.forEach(lambda);
    return SegmentMask(mask, {0,0});
}

/*!
 * \brief Метка сегмента.
 * \details Для указанной точки pos вовращает метку сегмента, котороиу она принадлежит.
 * \param pos - точка изображения
 * \return идентификатор сегмента (метка)
 */
int ConnectedComponents::label(cv::Point pos) const
{
    return _labels(pos);
}

/*!
 * \brief Перечень доступных сегментов.
 * \details Возвращает множество идентификаторов всех существующих сегментов.
 * \warning методы join, joinToEnviroment и combine меняют этот список.
 * Поэтому при обходе сегментов с данными методами могут возникать ошибки.
 * \return
 */
const std::set<int> &ConnectedComponents::labelsSet() const
{
    return _labels_set;
}

/*!
 * \brief Рейтинг площадей сегментов.
 * \details Составляет словарь с площадями частей сегментов, попадающих в roi.
 * По умолчанию вернет такой словарь для всей карты.
 * \param roi - охватывающий прямоугольник.
 * \return Возвращает словарь<метка, площадь>
 */
std::map<int, int> ConnectedComponents::areas(const cv::Rect roi) const
{
    if (roi == cv::Rect()) return _areas;
    std::map<int, int> roi_areas;
    cv::Mat1i img_roi = _labels(roi);
    for(int row = 0; row < img_roi.rows; ++row)
        for(int col = 0; col < img_roi.cols; ++col)
        {
            int label = img_roi(row,col);
            roi_areas[label] += 1;
        }
    return roi_areas;
}
/*
cv::Mat1i ConnectedComponents::adjacencyMatrix() const
{
    using namespace cv;
    Mat1i AdjMat = Mat1i::zeros(labelCount(),labelCount());
    _labels.forEach([&](int& pix, const int* pos)
    {
        int y = pos[0], x = pos[1];
        for (int ny=-1;ny<=1;++ny)
            for (int nx=-1;nx<=1;++nx)
            {
                Point n{x+nx, y+ny};
                if (Rect({0,0},size()).contains(n) && (pix !=_labels(n)))
                    ++AdjMat(pix,_labels(n));
            }
    });
    return AdjMat;
}*/

/*!
 * \brief Граница сегмента.
 * \details Возвращает словарь <метка соседа, множество точек контура> для данного сегмента.
 * \param label - метка сегмента.
 * \return словарь <метка соседа, множество точек контура>
 */
std::map<int, std::set<cv::Point> > ConnectedComponents::segmentBorder(int label) const
{
    using namespace std;
    using namespace cv;

    Rect seg_rect = boundingRect(label);
    Rect img_rect = Rect({0,0}, size());

    map<int,set<Point>> borders;

    for(int y = seg_rect.y - 1; y <= seg_rect.br().y + 1; ++y)
        for(int x = seg_rect.x - 1; x <= seg_rect.br().x + 1; ++x)
            if(img_rect.contains({x, y}) && (_labels({x, y}) == label))
                for (int j = -1; j <= 1; ++j)
                    for (int i = -1; i <= 1; ++i)
                    {
                        if(img_rect.contains({x + i, y + j}))
                        {
                            int border_label = _labels({x + i, y + j});
                            if (border_label != label)
                                borders[border_label].insert({x/* + i*/, y/* + j*/});
                        }
                    }
    return borders;
}
/*
 * \brief segmentGraphLinkList - метод, возвращающий граф, представленный в
 * виде словаря связей между сегментами. Таким образом ключ Vec2i отражает наличие
 * связи (общей границы) между 2мя сегментами, при этом на первом месте сегмент с
 * меньшей меткой. Значение int - мощность (длина) границы.
 * \return
 * \badcode
std::map<cv::Vec2i, int> ConnectedComponents::segmentGraphLinkList() const
{
    using namespace std;
    using namespace cv;

    vector<vector<Point>> contours;
    findContours(_labels,contours,noArray(),RETR_LIST,CHAIN_APPROX_NONE);

    Rect imrect({0,0}, _labels.size());
    map<Vec2i, int> links;

    for(const vector<Point>& contour: contours)
    {
        int label = _labels(contour[0]);

        for(Point p: contour)
            for(int ny=-1; ny<=1;++ny) for(int nx=-1; nx<=1;++nx)
            {
                Point np(p.x+nx,p.y+ny);
                if(imrect.contains(np) && _labels(np)!=label)
                {
                    int nlabel = _labels(np);
                    Vec2i link = (label<nlabel)?Vec2i{label,nlabel}:Vec2i{nlabel,label};
                    links[link]+=1;
                    nx = ny = 1;//break
                }
            }
    }
    return links;
}
 */
/*!
 * \brief Объединить сегменты.
 * \details Объединяет сегменты (не обязательно связные) в один с меткой label1.
 * При этом label2 становится не доступной.
 * \warning Меняет список доступных сегментов.
 * \param label1 - метка сегмента
 * \param label2 - метка присоединяемого сегмента
 */
void ConnectedComponents::join(int label1, int label2)
{
    using namespace cv;
    if (label1 == label2)
        throw std::invalid_argument("ConnectedComponents::join: label1 must not be equal label2");

    Rect rect1 = boundingRect(label1);
    int area1 = segmentArea(label1);

    Rect rect2 = boundingRect(label2);
    int area2 = segmentArea(label2);

    for (int y = rect2.y; y < rect2.br().y; ++y)
        for (int x = rect2.x; x < rect2.br().x; ++x)
        {
            int& pix = _labels({x, y});
            if (pix == label2) pix=label1;
        }

    Rect joined_rect = rect1 | rect2;
    _rects.at(label1) = joined_rect;
    _rects.erase(label2);

    int joined_area = area1 + area2;
    _areas.at(label1) = joined_area;
    _areas.erase(label2);
    _labels_set.erase(label2);
}

/*!
 * \brief Присоединить к окружению.
 * \details Присоединяет сегмент с меткой label к соседу с максимальной длинной общей границы, после
 * чего становится не доступным.
 * \warning отдельные сегменты фона (мелкие островки нулей) не будут удалены подобным методом.
 * \param label - метка сегмента
 * \return метка сегмента, к которому был присоединен сегмент.
 */
int ConnectedComponents::joinToEnviroment(int label)
{
    using namespace std;
    using namespace cv;

    auto borders = segmentBorder(label);

    int new_label = 0;
    size_t max_length = 0;
    for(const pair<int, set<Point>>& b: borders)
        if (b.second.size() > max_length)
        {
            new_label = b.first;
            max_length = b.second.size();
        }

    join(new_label, label);
    return new_label;
}
/*!
 * \brief Объединить мелкие в крупные.
 * \details Присоединяет все мелкие связные сегменты (в том числе связные сегменты фона),
 * к окружающим их крупным, если их площадь меньше area_threshold.
 * \warning При числе крупных сегментов после обработки > 255 возможно их спантанное объединение.
 * \param area_threshold - минимальная площадь сегмента
 */
void ConnectedComponents::combine(int area_threshold)
{
    /*
    list<int> combine_list;
    for (auto p: _areas)
    {
        int area = p.second;
        if (area < area_threshold)
            combine_list.push_back(p.first);
    }
    for (int id: combine_list)
        joinToEnviroment(id);*/

    using namespace cv;
    using namespace std;
    if (area_threshold < 1) return;
    vector<vector<Point>> contours;
    findContours(_labels, contours, noArray(), CV_RETR_FLOODFILL,CHAIN_APPROX_NONE);
    multimap<double, vector<Point>> contour_map;
    for(auto c: contours)
    {
        double area = contourArea(c);
        if (area >= area_threshold)
            contour_map.insert(make_pair(area,c));
    }

    Mat1i dst = Mat1i::zeros(_labels.rows, _labels.cols);
    int color = 1;
    for(auto It = contour_map.rbegin(); It != contour_map.rend(); ++It)
    {
        vector<vector<Point>> contour = {It->second};
        if(_labels(contour[0][0]) != 0)
            drawContours( dst, contour, 0, color++, FILLED, _connectivity);
        else
            drawContours( dst, contour, 0, 0, FILLED, _connectivity);
    }
    *this = ConnectedComponents(dst, _connectivity, _ccltype);//Warning dst not Mat1b!
}

/*!
 * \brief Размер карты сегментации.
 * \return ширина и высота.
 */
cv::Size ConnectedComponents::size() const
{
    return _labels.size();
}

/*!
 * \brief Отрисовка карты сегментации.
 * \details Для каждого идентификатора сегмента случайным образом генерируется цвет,
 * после чего отрисовывает карту, в виде залитых данными цветами областей.
 * \return
 */
cv::Mat3b ConnectedComponents::visualize() const
{
    using namespace std;
    using namespace cv;

    //переписать через findContours(_labels,contours,noArray(),RETR_FLOODFILL,CHAIN_APPROX_NONE);
    // for(int i=0; i<contours.size(); ++i)
    //      drawContours(img, contours, i, randColor());


    map<int,Vec3b> colors;
    int width = _labels.cols;
    int height = _labels.rows;

    for(int y = 0; y < height; ++y)
        for(int x = 0; x < width; ++x)
        {
            int pix = _labels({x, y});
            if (colors.find(pix) == colors.end())
                colors[pix] = {uchar(rand()%256), uchar(rand()%256), uchar(rand()%256)};
        }

    Mat3b ret(_labels.size());
    ret.forEach([&](Vec3b& pix, const int* pos)
    {
        int seg_id = _labels(pos);
        pix = colors[seg_id];
    });
    return ret;
}

/*!
 * \brief Нарисовать границы.
 * \details Аналогичен visualize, однако он лишь накладывает уникальные по цвету для каждого сегмента границы
 * с толщиной thickness в пикселях. Поэтому каждая граница между сегментами является двуцветной.
 * \param image
 * \param thickness
 */
void ConnectedComponents::drawBorders(cv::Mat3b &image, int thickness) const
{
    using namespace std;
    using namespace cv;
    int t = thickness;
    int width = image.cols, height = image.rows;
    Mat3b  visulization = visualize();
    visulization.forEach([&](Vec3b& pix, const int* pos)
    {
        int y = pos[0], x = pos[1];
        for (int ny = -t; ny <= t; ++ny)
            for (int nx =- t; nx <= t; ++nx)
            {
                Point n{x+nx, y+ny};
                if (!(0 <= n.x && n.x < width && 0 < n.y && n.y < height) || pix != visulization(n))
                {
                    image(pos) = pix;
                    nx = ny = t;//break;
                }
            }
    });
    return;
}

