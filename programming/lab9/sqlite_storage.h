#ifndef SQLITE_STORAGE_H
#define SQLITE_STORAGE_H

#include "storage.hpp"
#include <QSqlDatabase>
#include "user.h"

class SqliteStorage : public Storage {

    vector<Language> lang_;
    vector<Programmer> lang2_;

    const string dir_name_;
    QSqlDatabase db_;

    int user_id;

    int getNewId();

public:
    SqliteStorage(const string & dir_name);

    bool open();
    bool close();
    // Languages
    vector<Language> getAllLanguages(void);
    optional<Language> getLanguageById(int language_id);
    bool updateLanguage(const Language & language);
    bool removeLanguage(int language_id);
    int insertLanguage(const Language & language);
    // Programmer
    vector<Programmer> getAllProgrammer(void);
    optional<Programmer> getProgrammerById(int lprogrammer_id);
    bool updateProgrammer(const Programmer & programmer);
    bool removeProgrammer(int programmer_id);
    int insertProgrammer(const Programmer & programmer);

    bool login_pass();
    vector<Language> getYourLangs();
    vector<Programmer> getProgrammersForLang(const Language & lang);
    int insertProgrammerToLang(const Language & lang, const Programmer & programmer);
    bool removeProgrammerFromLang(const Language & lang, const Programmer & programmer);
    int returnId();

};

#endif // SQLITE_STORAGE_H
