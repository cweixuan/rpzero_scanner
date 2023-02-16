/*
 * Generated by gdbus-codegen 2.50.3. DO NOT EDIT.
 *
 * The license of this code is the same as for the source it was derived from.
 */

#ifndef ___HOME_PI_DOCUMENTS_RPZERO_SCANNER_BUILD_DBUS_ORG_BLUEZ_GATTCHARACTERISTIC1_H__
#define ___HOME_PI_DOCUMENTS_RPZERO_SCANNER_BUILD_DBUS_ORG_BLUEZ_GATTCHARACTERISTIC1_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for org.bluez.GattCharacteristic1 */

#define TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1 (org_bluez_gatt_characteristic1_get_type ())
#define ORG_BLUEZ_GATT_CHARACTERISTIC1(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1, OrgBluezGattCharacteristic1))
#define IS_ORG_BLUEZ_GATT_CHARACTERISTIC1(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1))
#define ORG_BLUEZ_GATT_CHARACTERISTIC1_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1, OrgBluezGattCharacteristic1Iface))

struct _OrgBluezGattCharacteristic1;
typedef struct _OrgBluezGattCharacteristic1 OrgBluezGattCharacteristic1;
typedef struct _OrgBluezGattCharacteristic1Iface OrgBluezGattCharacteristic1Iface;

struct _OrgBluezGattCharacteristic1Iface
{
  GTypeInterface parent_iface;



  gboolean (*handle_acquire_notify) (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *arg_options);

  gboolean (*handle_acquire_write) (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GUnixFDList *fd_list,
    GVariant *arg_options);

  gboolean (*handle_read_value) (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *arg_options);

  gboolean (*handle_start_notify) (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_stop_notify) (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_write_value) (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *arg_value,
    GVariant *arg_options);

  const gchar *const * (*get_descriptors) (OrgBluezGattCharacteristic1 *object);

  const gchar *const * (*get_flags) (OrgBluezGattCharacteristic1 *object);

  gboolean  (*get_notify_acquired) (OrgBluezGattCharacteristic1 *object);

  gboolean  (*get_notifying) (OrgBluezGattCharacteristic1 *object);

  const gchar * (*get_service) (OrgBluezGattCharacteristic1 *object);

  const gchar * (*get_uuid) (OrgBluezGattCharacteristic1 *object);

  const gchar * (*get_value) (OrgBluezGattCharacteristic1 *object);

  gboolean  (*get_write_acquired) (OrgBluezGattCharacteristic1 *object);

  void (*properties_changed) (
    OrgBluezGattCharacteristic1 *object,
    const gchar *arg_interface,
    GVariant *arg_changed_properties,
    const gchar *const *arg_invalidated_properties);

};

GType org_bluez_gatt_characteristic1_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *org_bluez_gatt_characteristic1_interface_info (void);
guint org_bluez_gatt_characteristic1_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void org_bluez_gatt_characteristic1_complete_read_value (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *value);

void org_bluez_gatt_characteristic1_complete_write_value (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

void org_bluez_gatt_characteristic1_complete_start_notify (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

void org_bluez_gatt_characteristic1_complete_stop_notify (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

void org_bluez_gatt_characteristic1_complete_acquire_write (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GUnixFDList *fd_list,
    GVariant *fd,
    guint16 mtu);

void org_bluez_gatt_characteristic1_complete_acquire_notify (
    OrgBluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *fd,
    guint16 mtu);



/* D-Bus signal emissions functions: */
void org_bluez_gatt_characteristic1_emit_properties_changed (
    OrgBluezGattCharacteristic1 *object,
    const gchar *arg_interface,
    GVariant *arg_changed_properties,
    const gchar *const *arg_invalidated_properties);



/* D-Bus method calls: */
void org_bluez_gatt_characteristic1_call_read_value (
    OrgBluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_gatt_characteristic1_call_read_value_finish (
    OrgBluezGattCharacteristic1 *proxy,
    GVariant **out_value,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_gatt_characteristic1_call_read_value_sync (
    OrgBluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GVariant **out_value,
    GCancellable *cancellable,
    GError **error);

void org_bluez_gatt_characteristic1_call_write_value (
    OrgBluezGattCharacteristic1 *proxy,
    GVariant *arg_value,
    GVariant *arg_options,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_gatt_characteristic1_call_write_value_finish (
    OrgBluezGattCharacteristic1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_gatt_characteristic1_call_write_value_sync (
    OrgBluezGattCharacteristic1 *proxy,
    GVariant *arg_value,
    GVariant *arg_options,
    GCancellable *cancellable,
    GError **error);

void org_bluez_gatt_characteristic1_call_start_notify (
    OrgBluezGattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_gatt_characteristic1_call_start_notify_finish (
    OrgBluezGattCharacteristic1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_gatt_characteristic1_call_start_notify_sync (
    OrgBluezGattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GError **error);

void org_bluez_gatt_characteristic1_call_stop_notify (
    OrgBluezGattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_gatt_characteristic1_call_stop_notify_finish (
    OrgBluezGattCharacteristic1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_gatt_characteristic1_call_stop_notify_sync (
    OrgBluezGattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GError **error);

void org_bluez_gatt_characteristic1_call_acquire_write (
    OrgBluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GUnixFDList *fd_list,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_gatt_characteristic1_call_acquire_write_finish (
    OrgBluezGattCharacteristic1 *proxy,
    GVariant **out_fd,
    guint16 *out_mtu,
    GUnixFDList **out_fd_list,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_gatt_characteristic1_call_acquire_write_sync (
    OrgBluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GUnixFDList  *fd_list,
    GVariant **out_fd,
    guint16 *out_mtu,
    GUnixFDList **out_fd_list,
    GCancellable *cancellable,
    GError **error);

void org_bluez_gatt_characteristic1_call_acquire_notify (
    OrgBluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean org_bluez_gatt_characteristic1_call_acquire_notify_finish (
    OrgBluezGattCharacteristic1 *proxy,
    GVariant **out_fd,
    guint16 *out_mtu,
    GAsyncResult *res,
    GError **error);

gboolean org_bluez_gatt_characteristic1_call_acquire_notify_sync (
    OrgBluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GVariant **out_fd,
    guint16 *out_mtu,
    GCancellable *cancellable,
    GError **error);



/* D-Bus property accessors: */
const gchar *org_bluez_gatt_characteristic1_get_uuid (OrgBluezGattCharacteristic1 *object);
gchar *org_bluez_gatt_characteristic1_dup_uuid (OrgBluezGattCharacteristic1 *object);
void org_bluez_gatt_characteristic1_set_uuid (OrgBluezGattCharacteristic1 *object, const gchar *value);

const gchar *org_bluez_gatt_characteristic1_get_service (OrgBluezGattCharacteristic1 *object);
gchar *org_bluez_gatt_characteristic1_dup_service (OrgBluezGattCharacteristic1 *object);
void org_bluez_gatt_characteristic1_set_service (OrgBluezGattCharacteristic1 *object, const gchar *value);

const gchar *org_bluez_gatt_characteristic1_get_value (OrgBluezGattCharacteristic1 *object);
gchar *org_bluez_gatt_characteristic1_dup_value (OrgBluezGattCharacteristic1 *object);
void org_bluez_gatt_characteristic1_set_value (OrgBluezGattCharacteristic1 *object, const gchar *value);

gboolean org_bluez_gatt_characteristic1_get_notifying (OrgBluezGattCharacteristic1 *object);
void org_bluez_gatt_characteristic1_set_notifying (OrgBluezGattCharacteristic1 *object, gboolean value);

const gchar *const *org_bluez_gatt_characteristic1_get_flags (OrgBluezGattCharacteristic1 *object);
gchar **org_bluez_gatt_characteristic1_dup_flags (OrgBluezGattCharacteristic1 *object);
void org_bluez_gatt_characteristic1_set_flags (OrgBluezGattCharacteristic1 *object, const gchar *const *value);

const gchar *const *org_bluez_gatt_characteristic1_get_descriptors (OrgBluezGattCharacteristic1 *object);
gchar **org_bluez_gatt_characteristic1_dup_descriptors (OrgBluezGattCharacteristic1 *object);
void org_bluez_gatt_characteristic1_set_descriptors (OrgBluezGattCharacteristic1 *object, const gchar *const *value);

gboolean org_bluez_gatt_characteristic1_get_write_acquired (OrgBluezGattCharacteristic1 *object);
void org_bluez_gatt_characteristic1_set_write_acquired (OrgBluezGattCharacteristic1 *object, gboolean value);

gboolean org_bluez_gatt_characteristic1_get_notify_acquired (OrgBluezGattCharacteristic1 *object);
void org_bluez_gatt_characteristic1_set_notify_acquired (OrgBluezGattCharacteristic1 *object, gboolean value);


/* ---- */

#define TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_PROXY (org_bluez_gatt_characteristic1_proxy_get_type ())
#define ORG_BLUEZ_GATT_CHARACTERISTIC1_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_PROXY, OrgBluezGattCharacteristic1Proxy))
#define ORG_BLUEZ_GATT_CHARACTERISTIC1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_PROXY, OrgBluezGattCharacteristic1ProxyClass))
#define ORG_BLUEZ_GATT_CHARACTERISTIC1_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_PROXY, OrgBluezGattCharacteristic1ProxyClass))
#define IS_ORG_BLUEZ_GATT_CHARACTERISTIC1_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_PROXY))
#define IS_ORG_BLUEZ_GATT_CHARACTERISTIC1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_PROXY))

typedef struct _OrgBluezGattCharacteristic1Proxy OrgBluezGattCharacteristic1Proxy;
typedef struct _OrgBluezGattCharacteristic1ProxyClass OrgBluezGattCharacteristic1ProxyClass;
typedef struct _OrgBluezGattCharacteristic1ProxyPrivate OrgBluezGattCharacteristic1ProxyPrivate;

struct _OrgBluezGattCharacteristic1Proxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  OrgBluezGattCharacteristic1ProxyPrivate *priv;
};

struct _OrgBluezGattCharacteristic1ProxyClass
{
  GDBusProxyClass parent_class;
};

GType org_bluez_gatt_characteristic1_proxy_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (OrgBluezGattCharacteristic1Proxy, g_object_unref)
#endif

void org_bluez_gatt_characteristic1_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
OrgBluezGattCharacteristic1 *org_bluez_gatt_characteristic1_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
OrgBluezGattCharacteristic1 *org_bluez_gatt_characteristic1_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void org_bluez_gatt_characteristic1_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
OrgBluezGattCharacteristic1 *org_bluez_gatt_characteristic1_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
OrgBluezGattCharacteristic1 *org_bluez_gatt_characteristic1_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_SKELETON (org_bluez_gatt_characteristic1_skeleton_get_type ())
#define ORG_BLUEZ_GATT_CHARACTERISTIC1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_SKELETON, OrgBluezGattCharacteristic1Skeleton))
#define ORG_BLUEZ_GATT_CHARACTERISTIC1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_SKELETON, OrgBluezGattCharacteristic1SkeletonClass))
#define ORG_BLUEZ_GATT_CHARACTERISTIC1_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_SKELETON, OrgBluezGattCharacteristic1SkeletonClass))
#define IS_ORG_BLUEZ_GATT_CHARACTERISTIC1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_SKELETON))
#define IS_ORG_BLUEZ_GATT_CHARACTERISTIC1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_ORG_BLUEZ_GATT_CHARACTERISTIC1_SKELETON))

typedef struct _OrgBluezGattCharacteristic1Skeleton OrgBluezGattCharacteristic1Skeleton;
typedef struct _OrgBluezGattCharacteristic1SkeletonClass OrgBluezGattCharacteristic1SkeletonClass;
typedef struct _OrgBluezGattCharacteristic1SkeletonPrivate OrgBluezGattCharacteristic1SkeletonPrivate;

struct _OrgBluezGattCharacteristic1Skeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  OrgBluezGattCharacteristic1SkeletonPrivate *priv;
};

struct _OrgBluezGattCharacteristic1SkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType org_bluez_gatt_characteristic1_skeleton_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (OrgBluezGattCharacteristic1Skeleton, g_object_unref)
#endif

OrgBluezGattCharacteristic1 *org_bluez_gatt_characteristic1_skeleton_new (void);


G_END_DECLS

#endif /* ___HOME_PI_DOCUMENTS_RPZERO_SCANNER_BUILD_DBUS_ORG_BLUEZ_GATTCHARACTERISTIC1_H__ */
