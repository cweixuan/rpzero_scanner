/*
 * This file is generated by gdbus-codegen, do not modify it.
 *
 * The license of this code is the same as for the D-Bus interface description
 * it was derived from. Note that it links to GLib, so must comply with the
 * LGPL linking clauses.
 */

#ifndef ___HOME_NOTPI_DOCUMENTS_RPZERO_SCANNER_BLALFRED_SCANNER_BUILD_DBUS_ORG_BLUEZ_DEVICE1_H__
#define ___HOME_NOTPI_DOCUMENTS_RPZERO_SCANNER_BLALFRED_SCANNER_BUILD_DBUS_ORG_BLUEZ_DEVICE1_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for org.bluez.Device1 */

#define TYPE_ORG_BLUEZ_DEVICE1 (org_bluez_device1_get_type ())
#define ORG_BLUEZ_DEVICE1(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_ORG_BLUEZ_DEVICE1, OrgBluezDevice1))
#define IS_ORG_BLUEZ_DEVICE1(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_ORG_BLUEZ_DEVICE1))
#define ORG_BLUEZ_DEVICE1_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), TYPE_ORG_BLUEZ_DEVICE1, OrgBluezDevice1Iface))

struct _OrgBluezDevice1;
typedef struct _OrgBluezDevice1 OrgBluezDevice1;
typedef struct _OrgBluezDevice1Iface OrgBluezDevice1Iface;

struct _OrgBluezDevice1Iface
{
  GTypeInterface parent_iface;



  gboolean (*handle_cancel_pairing) (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_connect) (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_connect_profile) (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_UUID);

  gboolean (*handle_disconnect) (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_disconnect_profile) (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_UUID);

  gboolean (*handle_pair) (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation);

  const gchar * (*get_adapter) (OrgBluezDevice1 *object);

  const gchar * (*get_address) (OrgBluezDevice1 *object);

  const gchar * (*get_address_type) (OrgBluezDevice1 *object);

  const gchar * (*get_alias) (OrgBluezDevice1 *object);

  guint16  (*get_appearance) (OrgBluezDevice1 *object);

  gboolean  (*get_blocked) (OrgBluezDevice1 *object);

  guint  (*get_class) (OrgBluezDevice1 *object);

  gboolean  (*get_connected) (OrgBluezDevice1 *object);

  const gchar *const * (*get_gatt_services) (OrgBluezDevice1 *object);

  const gchar * (*get_icon) (OrgBluezDevice1 *object);

  gboolean  (*get_legacy_pairing) (OrgBluezDevice1 *object);

  GVariant * (*get_manufacturer_data) (OrgBluezDevice1 *object);

  const gchar * (*get_modalias) (OrgBluezDevice1 *object);

  const gchar * (*get_name) (OrgBluezDevice1 *object);

  gboolean  (*get_paired) (OrgBluezDevice1 *object);

  gint16  (*get_rssi) (OrgBluezDevice1 *object);

  GVariant * (*get_service_data) (OrgBluezDevice1 *object);

  gboolean  (*get_services_resolved) (OrgBluezDevice1 *object);

  gboolean  (*get_trusted) (OrgBluezDevice1 *object);

  gint16  (*get_tx_power) (OrgBluezDevice1 *object);

  const gchar *const * (*get_uuids) (OrgBluezDevice1 *object);

  void (*properties_changed) (
    OrgBluezDevice1 *object,
    const gchar *arg_interface,
    GVariant *arg_changed_properties,
    const gchar *const *arg_invalidated_properties);

};

GType org_bluez_device1_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *org_bluez_device1_interface_info (void);
guint org_bluez_device1_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void org_bluez_device1_complete_disconnect (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation);

void org_bluez_device1_complete_connect (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation);

void org_bluez_device1_complete_connect_profile (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation);

void org_bluez_device1_complete_disconnect_profile (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation);

void org_bluez_device1_complete_pair (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation);

void org_bluez_device1_complete_cancel_pairing (
    OrgBluezDevice1 *object,
    GDBusMethodInvocation *invocation);



/* D-Bus signal emissions functions: */
void org_bluez_device1_emit_properties_changed (
    OrgBluezDevice1 *object,
    const gchar *arg_interface,
    GVariant *arg_changed_properties,
    const gchar *const *arg_invalidated_properties);



/* D-Bus method calls: */
void org_bluez_device1_call_disconnect (
    OrgBluezDevice1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_device1_call_disconnect_finish (
    OrgBluezDevice1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_device1_call_disconnect_sync (
    OrgBluezDevice1 *proxy,
    GCancellable *cancellable,
    GError **error);

void org_bluez_device1_call_connect (
    OrgBluezDevice1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_device1_call_connect_finish (
    OrgBluezDevice1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_device1_call_connect_sync (
    OrgBluezDevice1 *proxy,
    GCancellable *cancellable,
    GError **error);

void org_bluez_device1_call_connect_profile (
    OrgBluezDevice1 *proxy,
    const gchar *arg_UUID,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_device1_call_connect_profile_finish (
    OrgBluezDevice1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_device1_call_connect_profile_sync (
    OrgBluezDevice1 *proxy,
    const gchar *arg_UUID,
    GCancellable *cancellable,
    GError **error);

void org_bluez_device1_call_disconnect_profile (
    OrgBluezDevice1 *proxy,
    const gchar *arg_UUID,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_device1_call_disconnect_profile_finish (
    OrgBluezDevice1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_device1_call_disconnect_profile_sync (
    OrgBluezDevice1 *proxy,
    const gchar *arg_UUID,
    GCancellable *cancellable,
    GError **error);

void org_bluez_device1_call_pair (
    OrgBluezDevice1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_device1_call_pair_finish (
    OrgBluezDevice1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_device1_call_pair_sync (
    OrgBluezDevice1 *proxy,
    GCancellable *cancellable,
    GError **error);

void org_bluez_device1_call_cancel_pairing (
    OrgBluezDevice1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_device1_call_cancel_pairing_finish (
    OrgBluezDevice1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_device1_call_cancel_pairing_sync (
    OrgBluezDevice1 *proxy,
    GCancellable *cancellable,
    GError **error);



/* D-Bus property accessors: */
const gchar *org_bluez_device1_get_address (OrgBluezDevice1 *object);
gchar *org_bluez_device1_dup_address (OrgBluezDevice1 *object);
void org_bluez_device1_set_address (OrgBluezDevice1 *object, const gchar *value);

const gchar *org_bluez_device1_get_address_type (OrgBluezDevice1 *object);
gchar *org_bluez_device1_dup_address_type (OrgBluezDevice1 *object);
void org_bluez_device1_set_address_type (OrgBluezDevice1 *object, const gchar *value);

const gchar *org_bluez_device1_get_name (OrgBluezDevice1 *object);
gchar *org_bluez_device1_dup_name (OrgBluezDevice1 *object);
void org_bluez_device1_set_name (OrgBluezDevice1 *object, const gchar *value);

const gchar *org_bluez_device1_get_alias (OrgBluezDevice1 *object);
gchar *org_bluez_device1_dup_alias (OrgBluezDevice1 *object);
void org_bluez_device1_set_alias (OrgBluezDevice1 *object, const gchar *value);

guint org_bluez_device1_get_class (OrgBluezDevice1 *object);
void org_bluez_device1_set_class (OrgBluezDevice1 *object, guint value);

guint16 org_bluez_device1_get_appearance (OrgBluezDevice1 *object);
void org_bluez_device1_set_appearance (OrgBluezDevice1 *object, guint16 value);

const gchar *org_bluez_device1_get_icon (OrgBluezDevice1 *object);
gchar *org_bluez_device1_dup_icon (OrgBluezDevice1 *object);
void org_bluez_device1_set_icon (OrgBluezDevice1 *object, const gchar *value);

gboolean org_bluez_device1_get_paired (OrgBluezDevice1 *object);
void org_bluez_device1_set_paired (OrgBluezDevice1 *object, gboolean value);

gboolean org_bluez_device1_get_trusted (OrgBluezDevice1 *object);
void org_bluez_device1_set_trusted (OrgBluezDevice1 *object, gboolean value);

gboolean org_bluez_device1_get_blocked (OrgBluezDevice1 *object);
void org_bluez_device1_set_blocked (OrgBluezDevice1 *object, gboolean value);

gboolean org_bluez_device1_get_legacy_pairing (OrgBluezDevice1 *object);
void org_bluez_device1_set_legacy_pairing (OrgBluezDevice1 *object, gboolean value);

gint16 org_bluez_device1_get_rssi (OrgBluezDevice1 *object);
void org_bluez_device1_set_rssi (OrgBluezDevice1 *object, gint16 value);

gboolean org_bluez_device1_get_connected (OrgBluezDevice1 *object);
void org_bluez_device1_set_connected (OrgBluezDevice1 *object, gboolean value);

const gchar *const *org_bluez_device1_get_uuids (OrgBluezDevice1 *object);
gchar **org_bluez_device1_dup_uuids (OrgBluezDevice1 *object);
void org_bluez_device1_set_uuids (OrgBluezDevice1 *object, const gchar *const *value);

const gchar *org_bluez_device1_get_modalias (OrgBluezDevice1 *object);
gchar *org_bluez_device1_dup_modalias (OrgBluezDevice1 *object);
void org_bluez_device1_set_modalias (OrgBluezDevice1 *object, const gchar *value);

const gchar *org_bluez_device1_get_adapter (OrgBluezDevice1 *object);
gchar *org_bluez_device1_dup_adapter (OrgBluezDevice1 *object);
void org_bluez_device1_set_adapter (OrgBluezDevice1 *object, const gchar *value);

gint16 org_bluez_device1_get_tx_power (OrgBluezDevice1 *object);
void org_bluez_device1_set_tx_power (OrgBluezDevice1 *object, gint16 value);

GVariant *org_bluez_device1_get_manufacturer_data (OrgBluezDevice1 *object);
GVariant *org_bluez_device1_dup_manufacturer_data (OrgBluezDevice1 *object);
void org_bluez_device1_set_manufacturer_data (OrgBluezDevice1 *object, GVariant *value);

GVariant *org_bluez_device1_get_service_data (OrgBluezDevice1 *object);
GVariant *org_bluez_device1_dup_service_data (OrgBluezDevice1 *object);
void org_bluez_device1_set_service_data (OrgBluezDevice1 *object, GVariant *value);

gboolean org_bluez_device1_get_services_resolved (OrgBluezDevice1 *object);
void org_bluez_device1_set_services_resolved (OrgBluezDevice1 *object, gboolean value);

const gchar *const *org_bluez_device1_get_gatt_services (OrgBluezDevice1 *object);
gchar **org_bluez_device1_dup_gatt_services (OrgBluezDevice1 *object);
void org_bluez_device1_set_gatt_services (OrgBluezDevice1 *object, const gchar *const *value);


/* ---- */

#define TYPE_ORG_BLUEZ_DEVICE1_PROXY (org_bluez_device1_proxy_get_type ())
#define ORG_BLUEZ_DEVICE1_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_ORG_BLUEZ_DEVICE1_PROXY, OrgBluezDevice1Proxy))
#define ORG_BLUEZ_DEVICE1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_ORG_BLUEZ_DEVICE1_PROXY, OrgBluezDevice1ProxyClass))
#define ORG_BLUEZ_DEVICE1_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_ORG_BLUEZ_DEVICE1_PROXY, OrgBluezDevice1ProxyClass))
#define IS_ORG_BLUEZ_DEVICE1_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_ORG_BLUEZ_DEVICE1_PROXY))
#define IS_ORG_BLUEZ_DEVICE1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_ORG_BLUEZ_DEVICE1_PROXY))

typedef struct _OrgBluezDevice1Proxy OrgBluezDevice1Proxy;
typedef struct _OrgBluezDevice1ProxyClass OrgBluezDevice1ProxyClass;
typedef struct _OrgBluezDevice1ProxyPrivate OrgBluezDevice1ProxyPrivate;

struct _OrgBluezDevice1Proxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  OrgBluezDevice1ProxyPrivate *priv;
};

struct _OrgBluezDevice1ProxyClass
{
  GDBusProxyClass parent_class;
};

GType org_bluez_device1_proxy_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (OrgBluezDevice1Proxy, g_object_unref)
#endif

void org_bluez_device1_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
OrgBluezDevice1 *org_bluez_device1_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
OrgBluezDevice1 *org_bluez_device1_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void org_bluez_device1_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
OrgBluezDevice1 *org_bluez_device1_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
OrgBluezDevice1 *org_bluez_device1_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define TYPE_ORG_BLUEZ_DEVICE1_SKELETON (org_bluez_device1_skeleton_get_type ())
#define ORG_BLUEZ_DEVICE1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_ORG_BLUEZ_DEVICE1_SKELETON, OrgBluezDevice1Skeleton))
#define ORG_BLUEZ_DEVICE1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_ORG_BLUEZ_DEVICE1_SKELETON, OrgBluezDevice1SkeletonClass))
#define ORG_BLUEZ_DEVICE1_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_ORG_BLUEZ_DEVICE1_SKELETON, OrgBluezDevice1SkeletonClass))
#define IS_ORG_BLUEZ_DEVICE1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_ORG_BLUEZ_DEVICE1_SKELETON))
#define IS_ORG_BLUEZ_DEVICE1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_ORG_BLUEZ_DEVICE1_SKELETON))

typedef struct _OrgBluezDevice1Skeleton OrgBluezDevice1Skeleton;
typedef struct _OrgBluezDevice1SkeletonClass OrgBluezDevice1SkeletonClass;
typedef struct _OrgBluezDevice1SkeletonPrivate OrgBluezDevice1SkeletonPrivate;

struct _OrgBluezDevice1Skeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  OrgBluezDevice1SkeletonPrivate *priv;
};

struct _OrgBluezDevice1SkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType org_bluez_device1_skeleton_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (OrgBluezDevice1Skeleton, g_object_unref)
#endif

OrgBluezDevice1 *org_bluez_device1_skeleton_new (void);


G_END_DECLS

#endif /* ___HOME_NOTPI_DOCUMENTS_RPZERO_SCANNER_BLALFRED_SCANNER_BUILD_DBUS_ORG_BLUEZ_DEVICE1_H__ */
