#include "percentbargroup.h"
#include "bar_p.h"
#include "barlabel_p.h"
#include "separator_p.h"
#include <QDebug>

QTCOMMERCIALCHART_BEGIN_NAMESPACE


PercentBarGroup::PercentBarGroup(PercentBarChartSeries& series, QGraphicsItem *parent) :
    BarGroupBase(series, parent)
{
}

void PercentBarGroup::layoutChanged()
{
    // Scale bars to new layout
    // Layout for bars:
    if (mSeries.countRows() <= 0) {
        // Nothing to do.
        return;
    }

    // TODO: better way to auto-layout
    // Use reals for accurancy (we might get some compiler warnings... :)
    int count = mSeries.countColumns();
    int itemIndex(0);
    qreal tW = mWidth;
    qreal tC = count+1;
    qreal xStep = (tW/tC);
//    qreal xPos = ((tW/tC) + mBarDefaultWidth / 2);
    qreal xPos = ((tW/tC) - mBarDefaultWidth / 2);
    int labelIndex = mSeries.countColumns() * mSeries.countRows();

    for (int column = 0; column < mSeries.countColumns(); column++) {
        qreal colSum = mSeries.columnSum(column);
        qreal h = mHeight;
        qreal scale = (h / colSum);
        qreal yPos = h;
        for (int row=0; row < mSeries.countRows(); row++) {
            qreal barHeight = mSeries.valueAt(row, column) * scale;
            Bar* bar = reinterpret_cast<Bar*> (childItems().at(itemIndex));

            // TODO: width settable per bar?
            bar->resize(mBarDefaultWidth, barHeight);
            bar->setColor(mColors.at(row));
            bar->setPos(xPos, yPos-barHeight);
            itemIndex++;
            yPos -= barHeight;
        }

        // TODO: Layout for labels, remove magic number
        BarLabel* label = reinterpret_cast<BarLabel*> (childItems().at(labelIndex));
        label->setPos(xPos, mHeight + 20);
        labelIndex++;
        xPos += xStep;
    }

    // Position separators
    int separatorIndex = labelIndex;    // Separators are after labels in childItems(). TODO: better way to store these?
    xPos = xStep + xStep/2;             // Initial position is between first and second group. ie one and half steps from left.
    for (int s=0; s < mSeries.countColumns() - 1; s++) {
        Separator* sep = reinterpret_cast<Separator*> (childItems().at(separatorIndex));
        sep->setPos(xPos,0);
        sep->setSize(QSizeF(1,mHeight));
        xPos += xStep;
        separatorIndex++;
    }

    mLayoutDirty = true;
}

QTCOMMERCIALCHART_END_NAMESPACE
