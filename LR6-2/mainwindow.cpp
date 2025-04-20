#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QtCore>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->inputFileLabe->setText("Входной файл: D:\\qtworks\\LR6\\f.txt");
    ui->label_2->setText("Выходной файл: D:\\qtworks\\LR6\\g.txt");

    qDebug() << "Программа Вариант 6.2 готова к работе";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    const QString inputPath = "D:\\qtworks\\LR6\\f.txt";
    const QString outputPath = "D:\\qtworks\\LR6\\g.txt";

    qDebug() << "Обработка файлов:";
    qDebug() << "Входной:" << inputPath;
    qDebug() << "Выходной:" << outputPath;

    QFile inputFile(inputPath);
    QFile outputFile(outputPath);

    if (!inputFile.exists()) {
        QString error = QString("Входной файл %1 не найден").arg(inputPath);
        qDebug() << error;
        QMessageBox::critical(this, "Ошибка", error);
        return;
    }

    if (!inputFile.open(QIODevice::ReadOnly)) {
        QString error = QString("Не удалось открыть входной файл %1").arg(inputPath);
        qDebug() << error;
        QMessageBox::critical(this, "Ошибка", error);
        return;
    }

    if (!outputFile.open(QIODevice::WriteOnly)) {
        QString error = QString("Не удалось открыть выходной файл %1").arg(outputPath);
        qDebug() << error;
        inputFile.close();
        QMessageBox::critical(this, "Ошибка", error);
        return;
    }

    qDebug() << "Файлы успешно открыты";

    QTextStream in(&inputFile);
    QTextStream out(&outputFile);

    QString content = in.readAll().trimmed();
    QStringList numbersStr = content.split(" ", Qt::SkipEmptyParts);
    inputFile.close();

    qDebug() << "Прочитано чисел:" << numbersStr.size();

    if (numbersStr.isEmpty()) {
        qDebug() << "Входной файл пуст";
        QMessageBox::warning(this, "Ошибка", "Входной файл пуст");
        outputFile.close();
        return;
    }

    QList<int> numbers;
    bool conversionOk = true;
    for (const QString &numStr : numbersStr) {
        bool ok;
        int num = numStr.toInt(&ok);
        if (!ok) {
            conversionOk = false;
            break;
        }
        numbers.append(num);
    }

    if (!conversionOk || numbers.isEmpty()) {
        qDebug() << "Ошибка преобразования данных";
        QMessageBox::warning(this, "Ошибка", "Файл содержит некорректные числовые данные");
        outputFile.close();
        return;
    }

    qDebug() << "Успешно преобразовано чисел:" << numbers.size();

    QString processLog = "Процесс обработки:\n";
    for (int i = 0; i < numbers.size(); i += 5) {
        int end = qMin(i + 5, numbers.size());
        int maxInGroup = numbers[i];

        QString groupStr;
        for (int j = i; j < end; ++j) {
            if (numbers[j] > maxInGroup) maxInGroup = numbers[j];
            groupStr += QString::number(numbers[j]) + " ";
        }

        processLog += QString("Группа %1: %2 -> Максимум: %3\n")
                          .arg(i/5 + 1)
                          .arg(groupStr.trimmed())
                          .arg(maxInGroup);

        out << maxInGroup << " ";
    }

    outputFile.close();
    qDebug() << "Обработка завершена, результаты записаны в файл";

    QString outputContent;
    if (outputFile.open(QIODevice::ReadOnly)) {
        QTextStream outContent(&outputFile);
        outputContent = outContent.readAll().trimmed();
        outputFile.close();
    }

    QString report = QString(
                         "Отчет обработки файлов\n\n"
                         "Входной файл: %1\n"
                         "Содержимое:\n%2\n\n"
                         "%3\n"
                         "Выходной файл: %4\n"
                         "Результат:\n%5"
                         )
                         .arg(inputPath)
                         .arg(content)
                         .arg(processLog)
                         .arg(outputPath)
                         .arg(outputContent);

    ui->textEdit->setText(report);
    qDebug() << "Отчет сформирован и выведен";
}
