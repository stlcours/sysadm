// ===============================
//  PC-BSD REST/JSON API Server
// Available under the 3-clause BSD License
// Written by: Ken Moore <ken@pcbsd.org> July 2015
// =================================
#ifndef _PCBSD_REST_AUTHORIZATION_MANAGER_H
#define _PCBSD_REST_AUTHORIZATION_MANAGER_H

#include <QString>
#include <QHash>
#include <QJsonObject>
#include <QHostAddress>
#include <QDateTime>
#include <QByteArray>

class AuthorizationManager : public QObject{
	Q_OBJECT
public:
	AuthorizationManager();
	~AuthorizationManager();

	// == Token Interaction functions ==
	void clearAuth(QString token); //clear an authorization token
	bool checkAuth(QString token); //see if the given token is valid
	
	//SSL Certificate register/revoke/list (should only run if the current token is valid)
	bool RegisterCertificate(QString token, QString pubkey, QString nickname, QString email); //if token is valid, register the given cert for future logins
	bool RevokeCertificate(QString token, QString key, QString user=""); //user will be the current user if not empty - cannot touch other user's certs without full perms on current session
	void ListCertificates(QString token, QJsonObject *out);

	int checkAuthTimeoutSecs(QString token); //Return the number of seconds that a token is valid for

	//Stage 1 SSL Login Check: Generation of random string for this user
	QString GenerateEncCheckString(); 
	//Stage 2 SSL Login Check: Verify that the returned/encrypted string can be decoded and matches the initial random string
	QString LoginUC(QHostAddress host, QString encstring); 
	

private:
	QHash<QString, QDateTime> HASH;
	QHash <QString, QDateTime> IPFAIL;

	QString generateNewToken(bool isOperator, QString name);

	//Failure count management
	bool BumpFailCount(QString host);
	void ClearHostFail(QString host);

	//token->hashID filter simplification
	QString hashID(QString token){
	  QStringList tmp = QStringList(HASH.keys()).filter(token+"::::");
	  if(tmp.isEmpty()){ return ""; }
	  else{ return tmp.first(); }
	}
	
	//SSL Decrypt function
	QString DecryptSSLString(QString encstring, QString pubkey);
	
signals:
	void BlockHost(QHostAddress); //block a host address temporarily
	
};

#endif
