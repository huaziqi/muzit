#include "biliaudiometadatadialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

BiliAudioMetadataDialog::BiliAudioMetadataDialog(
    const BiliVideoInfo &videoInfo,
    const QVector<int> &partIndexes,
    QWidget *parent)
    : QDialog(parent), coverUrl(videoInfo.coverUrl)
{
    setWindowTitle(QStringLiteral("填写歌曲信息"));
    setMinimumWidth(420);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *hint = new QLabel(
        QStringLiteral("请确认每个分P对应的歌曲名和歌手。"));
    hint->setStyleSheet(QStringLiteral("color:#666;"));
    mainLayout->addWidget(hint);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    QWidget *content = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(content);

    for (int partIndex : partIndexes) {
        const BiliPlayUrlInfo &part = videoInfo.parts.at(partIndex);
        QGroupBox *group = new QGroupBox(
            QStringLiteral("P%1  %2").arg(part.page).arg(part.title));
        QFormLayout *form = new QFormLayout(group);

        QLineEdit *titleInput = new QLineEdit(
            videoInfo.parts.size() > 1 ? part.title : videoInfo.title);
        QLineEdit *artistInput = new QLineEdit(videoInfo.author);
        form->addRow(QStringLiteral("歌曲名"), titleInput);
        form->addRow(QStringLiteral("歌手"), artistInput);
        contentLayout->addWidget(group);

        editors.append(MetadataEditor{titleInput, artistInput});
        connect(titleInput, &QLineEdit::textChanged,
                this, [this] { updateAcceptButton(); });
        connect(artistInput, &QLineEdit::textChanged,
                this, [this] { updateAcceptButton(); });
    }

    contentLayout->addStretch(1);
    scrollArea->setWidget(content);
    mainLayout->addWidget(scrollArea);

    buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttons);

    updateAcceptButton();
}

QVector<AudioMetadata> BiliAudioMetadataDialog::metadata() const
{
    QVector<AudioMetadata> result;
    result.reserve(editors.size());
    for (const MetadataEditor &editor : editors) {
        AudioMetadata item;
        item.title = editor.titleInput->text().trimmed();
        item.artist = editor.artistInput->text().trimmed();
        item.coverUrl = coverUrl;
        result.append(item);
    }
    return result;
}

void BiliAudioMetadataDialog::updateAcceptButton()
{
    bool complete = !editors.isEmpty();
    for (const MetadataEditor &editor : editors) {
        if (editor.titleInput->text().trimmed().isEmpty()
            || editor.artistInput->text().trimmed().isEmpty()) {
            complete = false;
            break;
        }
    }

    if (buttons)
        buttons->button(QDialogButtonBox::Ok)->setEnabled(complete);
}
