#include "Network.h"

namespace NetworkKeys
{
  constexpr const char *SSID = "ssid";
  constexpr const char *PASSWORD = "password";
  constexpr const char *RSSI = "rssi";
  constexpr const char *ENCRYPTION_TYPE = "encryptionType";
  constexpr const char *CHANNEL = "channel";
  constexpr const char *BSSID = "bssid";
  constexpr const char *HIDDEN = "hidden";
}

Network::Network(const String &ssid,
                 const String &password,
                 int rssi,
                 int encryptionType,
                 int channel,
                 const String &bssid,
                 bool hidden)
    : ssid(ssid),
      password(password),
      rssi(rssi),
      encryptionType(toEncryptionType(encryptionType)),
      channel(channel),
      bssid(bssid),
      hidden(hidden)
{
}

Network::Network(const Network &other)
    : ssid(other.ssid),
      password(other.password),
      rssi(other.rssi),
      encryptionType(other.encryptionType),
      channel(other.channel),
      bssid(other.bssid),
      hidden(other.hidden)
{
}

gson::Str Network::toJson() const
{
  using namespace NetworkKeys;
  gson::Str j;
  j('{');
  j[SSID] = ssid;
  j[PASSWORD] = password;
  j[RSSI] = rssi;
  j[ENCRYPTION_TYPE] = int(encryptionType);
  j[CHANNEL] = channel;
  j[BSSID] = bssid;
  j[HIDDEN] = hidden;
  j('}');
  return j;
}

std::unique_ptr<JsonConvertible> Network::fromJson(const gson::Entry &e)
{
  using namespace NetworkKeys;

  if (!e.is(gson::Type::Object))
  {
    return std::make_unique<JsonError>("JSON is not an object");
  }

  const char *requiredFields[] = {SSID, PASSWORD, RSSI, ENCRYPTION_TYPE, CHANNEL, BSSID, HIDDEN};

  for (const char *field : requiredFields)
  {
    if (!e.has(field))
    {
      return std::make_unique<JsonError>(String("Missing required field: '") + field + "'");
    }
  }

  struct FieldTypeCheck
  {
    const char *name;
    gson::Type expectedType;
  };

  FieldTypeCheck checks[] = {
      {SSID, gson::Type::String},
      {PASSWORD, gson::Type::String},
      {RSSI, gson::Type::Int},
      {ENCRYPTION_TYPE, gson::Type::Int},
      {CHANNEL, gson::Type::Int},
      {BSSID, gson::Type::String},
      {HIDDEN, gson::Type::Bool}};

  for (const auto &check : checks)
  {
    if (!e[check.name].is(check.expectedType))
    {
      return std::make_unique<JsonError>(String("Field '") + check.name + "' has incorrect type");
    }
  }

  return std::make_unique<JsonParseSuccess<Network>>(
      Network(
          String(e[SSID].value()),
          String(e[PASSWORD].value()),
          int(e[RSSI].value()),
          int(e[ENCRYPTION_TYPE].value()),
          int(e[CHANNEL].value()),
          String(e[BSSID].value()),
          bool(e[HIDDEN].value())));
}

Network::EncryptionType Network::toEncryptionType(int enc)
{
  switch (enc)
  {
  case 7:
    return Network::EncryptionType::OPEN; // ENC_TYPE_NONE
  case 5:
    return Network::EncryptionType::WEP; // ENC_TYPE_WEP
  case 2:
    return Network::EncryptionType::WPA; // ENC_TYPE_TKIP
  case 4:
    return Network::EncryptionType::WPA2; // ENC_TYPE_CCMP
  case 8:
    return Network::EncryptionType::UNKNOWN; // ENC_TYPE_AUTO
  default:
    return Network::EncryptionType::UNKNOWN;
  }
}
