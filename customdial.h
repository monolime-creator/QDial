#ifndef CUSTOMDIAL_H
#define CUSTOMDIAL_H
#include <QWidget>
#include <QPainter>
#include <QLabel>
#include "stylescolors.h"


// Компонент крутилка в виджет помещена окружность по которой происходит переключение
// позиций. Ось X компонента направлена из центра виджета налево, Ось Y направлена из
// центра вверх. Диапазон углов лежит в пределах 0..360.
class CustomDial : public QWidget
{

    Q_OBJECT
public:
    //Конструктор
    explicit CustomDial(QWidget *parent = nullptr);

    Q_PROPERTY(float currentValue          READ getcurrentValue    WRITE setCurrentValue    NOTIFY currentValueChanged   ) //Текущее значение слайдера
    Q_PROPERTY(float defaultValue          READ getdefaultValue    WRITE setdefaultValue    NOTIFY defaultValueChanged   ) //Дефолтное значение слайдера
    Q_PROPERTY(float minBorder             READ getminBorder       WRITE setminBorder       NOTIFY minBorderChanged      ) //Минимальная граница значений
    Q_PROPERTY(float maxBorder             READ getmaxBorder       WRITE setMaxBorder       NOTIFY maxBorderChanged      ) //Максимальная граница значений

    Q_PROPERTY(float singleStep            READ getsingleStep      WRITE setSingleStep      NOTIFY singleStepChanged     ) //Переключение значений на стрелки

    Q_PROPERTY(float startAngle            READ getstartAngle      WRITE setstartAngle      NOTIFY startAngleChanged     ) //Начальный угол отрисовки
    Q_PROPERTY(float endAngle              READ getendAngle        WRITE setendAngle        NOTIFY endAngleChanged       ) //Конечный угол отрисовки
    Q_PROPERTY(float lineAngle             READ getlineAngle       WRITE setlineAngle       NOTIFY lineAngleChanged      ) //Отображение риски каждый (lineAngle) градус

    //Перечисление для выбора расчёта текущего значения относительно угла
    enum TYPE_CALC {First,Second};

    //----------------------STRUCTS-------------------
    //Структура цветовой палитры
    typedef struct{
      QColor SwithcerCircleleColorOff;
      QColor SwithcerCircleleColorOn;
      QColor BackGroundColor;
      QColor CentralTextColor;
      QColor TextColor;
      QColor CentralCircleColor;
      QColor MainCircleColor;
      QColor BorderColor;
    }Colors;
    //-------------------------------------------------

    //Перечисление типов стилей
    enum TYPE_STYLE
    {
      Dracula = 1,
      SnowWhite   = 2
    };

    //Getters
    float getminBorder()    const
    {
        return m_minBorder;
    }
    float getmaxBorder()    const
    {
        return m_maxBorder;
    }
    float getsingleStep()   const
    {
        return m_singleStep;
    }
    float getcurrentValue() const
    {
        return m_currentValue;
    }
    float getstartAngle()   const
    {
      return m_startAngle;
    }
    float getendAngle()     const
    {
      return m_endAngle;
    }
    qreal getlineAngle()    const
    {
        return m_lineAngle;
    }
    float getdefaultValue() const
    {
      return m_defaultValue;
    }
    TYPE_STYLE getStyle()   const {return m_style;}

    //---------------PUBLIC FUNCTIONS-----------------
    //Функция коррекции отрицательного диапазона углов
    void CorrectNegativeRange();
    //Установка дефолтных значений для компонента
    void  setDefaultValues(float defaultValue, float minBorder = 0,float MaxBorder = 100,
                          unsigned posAmount = 12, float sStep = 1,  float minAngle = 0, float maxAngle = 180);
    //Функция установки текущего стиля отрисовки
    void setStyle(const TYPE_STYLE &style);
    //------------------------------------------------

public slots:

    //Setters
    void setminBorder    (float minBorder)
    {
        m_minBorder = minBorder;
        emit minBorderChanged(m_minBorder);
    }
    void setMaxBorder    (float maxBorder)
    {
        m_maxBorder = maxBorder;
        emit maxBorderChanged(m_maxBorder);
    }
    void setSingleStep   (float singleStep)
    {
      //Ограничение задаваемого шага движения на Wheel and Key
      if(singleStep <= getRange()/4)
        m_singleStep = singleStep;
      else m_singleStep = 1;

      emit singleStepChanged(m_singleStep);
    }
    void setCurrentValue (float currentValue)
    {
        m_currentValue = currentValue;
        //Перевод из текущего значения в угол
        ValueToAngle();
        emit currentValueChanged(m_currentValue);
    }
    void setstartAngle   (float startAngle)
    {
      m_startAngle = startAngle;
      emit startAngleChanged(m_startAngle);
    }
    void setendAngle     (float endAngle)
    {
      m_endAngle = endAngle;
      emit endAngleChanged(m_endAngle);
    }
    void setlineAngle    (float lineAngle)
    {
        m_lineAngle = lineAngle;
        emit lineAngleChanged(m_lineAngle);
    }
    void setdefaultValue (float defaultValue)
    {
      //Проверка на вхождение дефолтного значения в заданные границы
      if(defaultValue <= getmaxBorder() && defaultValue >= getminBorder())
      {
        m_defaultValue = defaultValue;
      }
      else
      {
        m_defaultValue = m_minBorder;
      }

      setCurrentValue(m_defaultValue);
      emit defaultValueChanged(m_defaultValue);
    }

signals:

    //Signals
    void minBorderChanged    (float minBorder);
    void maxBorderChanged    (float maxBorder);
    void singleStepChanged   (float singleStep);
    void currentValueChanged (float currentValue);
    void lineAngleChanged    (float lineAngle);
    void startAngleChanged   (float startAngle);
    void endAngleChanged     (float endAngle);
    void defaultValueChanged (float defaultValue);

private:

    // Выбор стиля
    TYPE_STYLE m_style;
    // Минмальная и максимальные границы значений
    float m_minBorder;
    float m_maxBorder;
    // Еденичный шаг
    float m_singleStep;
    // Текущее значение
    float m_currentValue;
    // Дефолтное значение
    float m_defaultValue;
    //Инструмент для отрисовки
    QPainter *painter;
    //Координаты центра компонента
    qreal CenterX;
    qreal CenterY;
    //Объект цветов
    Colors m_colors;
    //Начальный угол
    float m_startAngle;
    //Конечный угол
    float m_endAngle;
    //Угол отрисовки рисок
    float m_lineAngle;

    // Диапазон значений
    float m_range;
    void  setRange(float _range){m_range = _range;}
    float getRange(){return m_range;}

    // Диапазон углов в пределах которых лежат значения
    float m_rangeAnlge;
    float getm_rangeAngle(){return m_rangeAnlge;}
    void  setm_rangeAngle(float _Angle){m_rangeAnlge = _Angle;}

    //Кол-во задаваемых рисок
    unsigned m_Amount;
    unsigned getm_Amount(){return m_Amount;}
    void setm_Amount(unsigned _Amount){
      if(_Amount >= 2 && _Amount <= 24)
        m_Amount = _Amount;
      else m_Amount = 2;
    }

    //Описание основной окружности
    float m_sz;
    float getm_sz(){return m_sz;}
    void  setm_sz(float _Value){m_sz = _Value;}

    //Флаг для отлавливания Events на клик и движение мыши
    bool m_pressed;
    bool getm_pressed(){return m_pressed;}
    void setm_pressed(bool _Value){m_pressed = _Value;}

    //Угол отрисовки малого круга
    float m_currentAngle;
    float getm_currentAngle(){return m_currentAngle;}
    void  setm_currentAngle(float _Angle){m_currentAngle = _Angle;}  

    //---------------------PRIVATE FUNCTIONS----------------------
    //Функция отрисовки маленькой окружности для изменения значений
    void DrawSwitchIndicator();
    //Функция отрисовки градиента
    void MainCircle();
    //Функция расчёта текущего угла по измененному значению
    void ValueToAngle();
    //Функция расчёта текущего значения по измененному углу
    void AngleToValue(float Angle, unsigned int TypeCalc);
    //-------------------------------------------------------------

protected:

    //Events
    void mouseDoubleClickEvent(QMouseEvent *event) override; //Срабатывание на двойной клик мыши
    void mousePressEvent(QMouseEvent *event)       override; //Срабатывание на нажатие мыши на компоненте
    void mouseReleaseEvent(QMouseEvent *event)     override; //Срабатывание на отжатие кнопки мыши
    void mouseMoveEvent (QMouseEvent *event)       override; //Срабатывание на движение мыши по компоненту
    void keyPressEvent  (QKeyEvent *_event)        override; //Срабатывание на нажатия кнопки (стрелки, пад)
    void keyReleaseEvent(QKeyEvent *_event)        override; //Срабатывание на отжатие кнопки (стрелки, пад)
    void wheelEvent     (QWheelEvent *_event)      override; //Срабатывание на прокрутку колеса мыши
    void paintEvent     (QPaintEvent *event)       override; //Срабатывание на отрисовку формы
    void resizeEvent    (QResizeEvent *event)      override; //Срабатывание на изменения размера формы компонента

};


#endif // CUSTOMDIAL_H
