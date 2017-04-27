MyAdBlocker est un proxy bloqueur de publicité créé par Nicolas Ferré et Théo Lognoz.

Compilation :
- Exécuter "make" (dans le dossier des sources) pour compiler le proxy
- Exécuter "make clean" (dans le dossier des sources) pour supprimer les fichiers temporaires et l'exécutable

Exécution :
- Configurer le navigateur pour qu'il se connecte directement à l'application 
	-> Sur firefox, aller dans Menu > Options > Avancé > Réseaux > Paramètres (rubrique "Connexion")
	-> Editer manuellement le proxy en mettant 127.0.0.1 comme adresse et 12345 comme port
	-> Recopier les informations pour la ligne "Proxy SSL" afin de profiter du blocage de publicité pour les pages https
- Lancer "./MyAdBlocker" (dans le dossier des sources) pour exécuter le programme
- Quand une publicité sera bloquée, un message apparaitra dans la console en donnant l'adresse du lien bloqué