#include "xml_storage.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <QString>
#include <QFile>
#include <QDebug>
#include <QtXml>

Language domElementToLanguage(QDomElement & element) {
    Language l;
    l.id = element.attribute("id").toInt();
    l.name = element.attribute("name").toStdString();
    l.type = element.attribute("type").toStdString();
    l.author = element.attribute("author").toStdString();

    return  l;
}

QDomElement langToDomElement(QDomDocument & doc, Language & l) {
    QDomElement lang_el = doc.createElement("lang");
    lang_el.setAttribute("id", l.id);
    lang_el.setAttribute("name", l.name.c_str());
    lang_el.setAttribute("type", l.type.c_str());
    lang_el.setAttribute("author", l.author.c_str());

    return  lang_el;
}

Programmer domElementToProg(QDomElement & element) {
    Programmer l;
    l.id = element.attribute("id").toInt();
    l.name = element.attribute("name").toStdString();
    l.stage = element.attribute("stage").toStdString();
    l.date_of_start = element.attribute("date_of_start").toStdString();

    return  l;
}

QDomElement progToDomElement(QDomDocument & doc, Programmer & l) {
    QDomElement lang_el = doc.createElement("lang");
    lang_el.setAttribute("id", l.id);
    lang_el.setAttribute("name", l.name.c_str());
    lang_el.setAttribute("stage", l.stage.c_str());
    lang_el.setAttribute("date_of_start", l.date_of_start.c_str());

    return  lang_el;
}

using namespace std;

bool XmlStorage::load() {
    string filename = this -> dir_name_ + "lang.xml";
    QString q_filename = QString::fromStdString(filename);
    QFile file(q_filename);
    bool is_opened = file.open(QFile::ReadOnly);
    if (!is_opened) {
        qDebug() << "file not opened: " << q_filename;
    }
    QTextStream ts(&file);
    QString text = ts.readAll();
    file.close();

    QDomDocument doc;
    QString errorMessage;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(text, &errorMessage, &errorLine, &errorColumn)) {
        qDebug() << "Error parsing XML text: " << errorMessage;
        qDebug() << "at line: " << errorLine;
        qDebug() << "and column: " << errorColumn;
    }

    QDomElement root = doc.documentElement();

    for (int i = 0; i < root.childNodes().size(); i++) {
        QDomNode node = root.childNodes().at(i);
        QDomElement el = node.toElement();
        Language l = domElementToLanguage(el);
        this -> lang_.push_back(l);
    }

    //

    string filename2 = this -> dir_name_ + "prog.xml";
    QString q_filename2 = QString::fromStdString(filename2);
    QFile file2(q_filename2);
    bool is_opened2 = file2.open(QFile::ReadOnly);
    if (!is_opened2) {
        qDebug() << "file not opened: " << q_filename2;
    }
    QTextStream ts2(&file2);
    QString text2 = ts2.readAll();
    file2.close();

    QDomDocument doc2;
    QString errorMessage2;
    int errorLine2;
    int errorColumn2;
    if (!doc2.setContent(text2, &errorMessage2, &errorLine2, &errorColumn2)) {
        qDebug() << "Error parsing XML text: " << errorMessage2;
        qDebug() << "at line: " << errorLine2;
        qDebug() << "and column: " << errorColumn2;
    }

    QDomElement root2 = doc2.documentElement();

    for (int i = 0; i < root2.childNodes().size(); i++) {
        QDomNode node = root2.childNodes().at(i);
        QDomElement el = node.toElement();
        Programmer l = domElementToProg(el);
        this -> lang2_.push_back(l);
    }

    return true;
}

bool XmlStorage::save() {
    QDomDocument doc;

    QDomElement root = doc.createElement("langs");

    for (Language & l : this -> lang_) {
        root.appendChild(langToDomElement(doc, l));
    }
    doc.appendChild(root);

    QString xml_text = doc.toString(4);

    string filename = this -> dir_name_ + "lang.xml";
    QString q_filename = QString::fromStdString(filename);
    QFile file(q_filename);
    bool is_opened = file.open(QFile::WriteOnly);
    if (!is_opened) {
        qDebug() << "file not opened: " << q_filename;
    }
    QTextStream ts(&file);
    ts << xml_text;
    file.close();

    QDomDocument doc2;

    QDomElement root2 = doc2.createElement("progs");

    for (Programmer & l : this -> lang2_) {
        root2.appendChild(progToDomElement(doc2, l));
    }
    doc2.appendChild(root2);

    QString xml_text2 = doc2.toString(4);

    string filename2 = this -> dir_name_ + "prog.xml";
    QString q_filename2 = QString::fromStdString(filename2);
    QFile file2(q_filename2);
    bool is_opened2 = file2.open(QFile::WriteOnly);
    if (!is_opened2) {
        qDebug() << "file not opened: " << q_filename2;
    }
    QTextStream ts2(&file2);
    ts2 << xml_text2;
    file2.close();

    return true;
}

vector<Language> XmlStorage::getAllLanguages(void) {
    return this -> lang_;
}

bool XmlStorage::updateLanguage(const Language & language) {

    for (Language & l : this -> lang_) {
        if (l.id == language.id) {
            l = language;
        }
    }

    return true;
}

bool XmlStorage::removeLanguage(int language_id) {
    int index = -1;
    for (int i = 0; i < this -> lang_.size(); i++) {
        if (this -> lang_[i].id == language_id) {
            index = i;
        }
    }
    if (index >= 0) {
        this -> lang_.erase(this -> lang_.begin() + index);
        return true;
    }
    return false;
}

int XmlStorage::getNewId() {
    int max_id = 0;
    for (Language & l : this -> lang_) {
        if (l.id >= max_id) {
            max_id = l.id;
        }
    }
    int new_id = max_id + 1;
    return new_id;
}

int XmlStorage::insertLanguage(const Language & language) {
    int new_id = this -> getNewId();
    Language copy = language;
    copy.id = new_id;
    this -> lang_.push_back(copy);
    return new_id;
}

optional<Language> XmlStorage::getLanguageById(int language_id) {
    for (Language & l : this -> lang_) {
        if (l.id == language_id) {
            return l;
        }
    }

    return nullopt;
}

vector<Programmer> XmlStorage::getAllProgrammer(void) {
    return this -> lang2_;
}

bool XmlStorage::updateProgrammer(const Programmer & programmer) {

    for (Programmer & l : this -> lang2_) {
        if (l.id == programmer.id) {
            l = programmer;
        }
    }

    return true;
}

bool XmlStorage::removeProgrammer(int programmer_id) {
    int index = -1;
    for (int i = 0; i < this -> lang2_.size(); i++) {
        if (this -> lang2_[i].id == programmer_id) {
            index = i;
        }
    }
    if (index >= 0) {
        this -> lang2_.erase(this -> lang2_.begin() + index);
        return true;
    }
    return false;
}

int XmlStorage::getNewProgId() {
    int max_id = 0;
    for (Programmer & l : this -> lang2_) {
        if (l.id >= max_id) {
            max_id = l.id;
        }
    }
    int new_id = max_id + 1;
    return new_id;
}

int XmlStorage::insertProgrammer(const Programmer & programmer) {
    int new_id = this -> getNewProgId();
    Programmer copy = programmer;
    copy.id = new_id;
    this -> lang2_.push_back(copy);
    return new_id;
}

optional<Programmer> XmlStorage::getProgrammerById(int programmer_id) {
    for (Programmer & l : this -> lang2_) {
        if (l.id == programmer_id) {
            return l;
        }
    }
    return nullopt;
}
