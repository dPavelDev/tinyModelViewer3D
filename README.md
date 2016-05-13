﻿# tinyModelViewer3D

**Группа**: 553501

**Разработчик**: Дорошкевич Павел Евгеньевич

**Тема**: Масштабирование и вращение 3D объектов.


### Описание проекта: 

1) Вращение и масштабирование 3D моделей

2) Возможность загрузки объектов из файла.

3) Используемые библиотеки: Qt 5.6

4) Поддерживаемые системы: Windows (32-bit, 64-bit), Linux

### Компиляция (на примере Ubuntu):

1) Установим необходимые зависимости:
	sudo apt-get install qt5-default
	sudo apt-get install qt5-image-formats-plugins
	sudo apt-get install libgl1-mesa-dev 
	sudo apt-get install libglu1-mesa-dev 
	
2) Скопируем необходимые исходные файлы:
	git clone https://github.com/dPavelDev/tinyModelViewer3D
	cd tinyModelViewer3D

3) Скомпилируем:
	qmake
	make -j8
	
4) Удалим лишние файлы:
	make clean


### История изменений:

#### 13.05.2016
	1) Добавлен прогресс загрузки модели с подробной статистикой
	2) Доработана функция освещения: если полигоны не содержат нормалей, то программа их автоматически добавит.
	3) Значительно оптимизирован рендер путем использования VBO

#### 04.05.2016
	1) Добавлена поддержка компилятора MSVC
	2) Улучшена поддержка материалов, ускорено чтение моделей
	3) Добавлена свободная камера, а также возможность выравнивания вида
	4) В заголовке окна отображение Fps
	5) Исправлена критическая ошибка, приводящая к потери производительности или отказу запуска на некоторых видеокартах
	6) Ликвидированы небольшие утечки памяти
	7) Улучшено поведение камеры
	8) Реализован выбор цвета фона, по умолчанию совпадает со стандартным фоном текущей темы в системе

#### 18.04.2016:

	1) Добавлена возможность отображения текстур на объектах
	2) Добавлена возможность загрузки объектов из файла (формат *.obj) с материалами (формат *.mtl) и текстурами (форматы *.bmp, *.jpg, *.png и др.)
	3) Реализована простая модель освещения.
	

#### 04.04.2016:
	Первая версия проекта
