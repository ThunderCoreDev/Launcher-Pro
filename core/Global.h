#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtGlobal>

#ifdef THUNDERNET_STATIC
#define THUNDERNET_EXPORT
#else
#ifdef THUNDERNET_LIBRARY
#define THUNDERNET_EXPORT Q_DECL_EXPORT
#else
#define THUNDERNET_EXPORT Q_DECL_IMPORT
#endif
#endif

// Definiciones globales
#define THUNDERNET_VERSION "1.0.0"
#define THUNDERNET_ORG "ThunderCore WoW"
#define THUNDERNET_DOMAIN "thundercore.com"

// Tipos comunes
using AccountId = quint32;
using CharacterGuid = quint64;

#endif // GLOBAL_H