#ifndef BILIAUDIOMETADATADIALOG_H
#define BILIAUDIOMETADATADIALOG_H

#include "bilitypes.h"

#include <QDialog>
#include <QLineEdit>
#include <QVector>

class QDialogButtonBox;

class BiliAudioMetadataDialog : public QDialog
{
public:
    explicit BiliAudioMetadataDialog(
        const BiliVideoInfo &videoInfo,
        const QVector<int> &partIndexes,
        QWidget *parent = nullptr);

    QVector<AudioMetadata> metadata() const;

private:
    struct MetadataEditor
    {
        QLineEdit *titleInput;
        QLineEdit *artistInput;
    };

    QVector<MetadataEditor> editors;
    QString coverUrl;
    QDialogButtonBox *buttons;

    void updateAcceptButton();
};

#endif // BILIAUDIOMETADATADIALOG_H
