#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <cmath>
#include <QDebug>
#include <QtCore>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "Программа Вариант 6.1 готова к работе";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    const QString filePath = "D:\\qtworks\\LR6\\LR6-1\\f.txt";
    qDebug() << "Путь к файлу:" << filePath;

    QFile file(filePath);
    qDebug() << "Попытка открыть файл:" << filePath;

    if (!file.exists()) {
        QString error = QString("Файл %1 не найден").arg(filePath);
        qDebug() << error;
        QMessageBox::critical(this, "Ошибка", error);
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        QString error = QString("Не удалось открыть файл %1").arg(filePath);
        qDebug() << error;
        QMessageBox::critical(this, "Ошибка", error);
        return;
    }

    qDebug() << "Файл успешно открыт, читаем данные...";

    QTextStream in(&file);
    QString content = in.readAll().trimmed();
    QStringList numbersStr = content.split(" ", Qt::SkipEmptyParts);
    file.close();

    qDebug() << "Прочитано чисел:" << numbersStr.size();

    if (numbersStr.isEmpty()) {
        qDebug() << "Файл пуст";
        QMessageBox::warning(this, "Ошибка", "Файл пуст или содержит некорректные данные");
        return;
    }

    QList<double> numbers;
    bool conversionOk = true;
    for (const QString &numStr : numbersStr) {
        bool ok;
        double num = numStr.toDouble(&ok);
        if (!ok) {
            conversionOk = false;
            break;
        }
        numbers.append(num);
    }

    if (!conversionOk || numbers.isEmpty()) {
        qDebug() << "Ошибка преобразования данных";
        QMessageBox::warning(this, "Ошибка", "Файл содержит некорректные числовые данные");
        return;
    }

    qDebug() << "Успешно преобразовано чисел:" << numbers.size();

    double maxElement = numbers[0];
    for (double num : numbers) {
        if (num > maxElement) maxElement = num;
    }

    double minEvenIndexElement = 0;
    bool hasEven = false;
    if (numbers.size() > 1) {
        minEvenIndexElement = numbers[1];
        hasEven = true;
        for (int i = 3; i < numbers.size(); i += 2) {
            if (numbers[i] < minEvenIndexElement) {
                minEvenIndexElement = numbers[i];
            }
        }
    }

    double maxAbsOddIndexElement = abs(numbers[0]);
    for (int i = 2; i < numbers.size(); i += 2) {
        if (abs(numbers[i]) > maxAbsOddIndexElement) {
            maxAbsOddIndexElement = abs(numbers[i]);
        }
    }

    double firstLastDiff = numbers.first() - numbers.last();

    QString report = QString(
                         "Отчет по анализу файла: %1\n\n"
                         "Содержимое файла:\n%2\n\n"
                         "Результаты анализа:\n"
                         "а) Наибольший компонент: %3\n"
                         "б) Наименьший компонент с четным номером: %4\n"
                         "в) Наибольший модуль компонента с нечетным номером: %5\n"
                         "г) Разность первого и последнего компонента: %6"
                         )
                         .arg(filePath)
                         .arg(content)
                         .arg(maxElement)
                         .arg(hasEven ? QString::number(minEvenIndexElement) : "нет")
                         .arg(maxAbsOddIndexElement)
                         .arg(firstLastDiff);

    ui->textEdit->setText(report);
    qDebug() << "Анализ завершен, результаты выведены";
}
