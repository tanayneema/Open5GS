
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "modification_notification.h"

OpenAPI_modification_notification_t *OpenAPI_modification_notification_create(
    OpenAPI_list_t *notify_items
)
{
    OpenAPI_modification_notification_t *modification_notification_local_var = ogs_malloc(sizeof(OpenAPI_modification_notification_t));
    ogs_assert(modification_notification_local_var);

    modification_notification_local_var->notify_items = notify_items;

    return modification_notification_local_var;
}

void OpenAPI_modification_notification_free(OpenAPI_modification_notification_t *modification_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == modification_notification) {
        return;
    }
    if (modification_notification->notify_items) {
        OpenAPI_list_for_each(modification_notification->notify_items, node) {
            OpenAPI_notify_item_free(node->data);
        }
        OpenAPI_list_free(modification_notification->notify_items);
        modification_notification->notify_items = NULL;
    }
    ogs_free(modification_notification);
}

cJSON *OpenAPI_modification_notification_convertToJSON(OpenAPI_modification_notification_t *modification_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (modification_notification == NULL) {
        ogs_error("OpenAPI_modification_notification_convertToJSON() failed [ModificationNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!modification_notification->notify_items) {
        ogs_error("OpenAPI_modification_notification_convertToJSON() failed [notify_items]");
        return NULL;
    }
    cJSON *notify_itemsList = cJSON_AddArrayToObject(item, "notifyItems");
    if (notify_itemsList == NULL) {
        ogs_error("OpenAPI_modification_notification_convertToJSON() failed [notify_items]");
        goto end;
    }
    OpenAPI_list_for_each(modification_notification->notify_items, node) {
        cJSON *itemLocal = OpenAPI_notify_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_modification_notification_convertToJSON() failed [notify_items]");
            goto end;
        }
        cJSON_AddItemToArray(notify_itemsList, itemLocal);
    }

end:
    return item;
}

OpenAPI_modification_notification_t *OpenAPI_modification_notification_parseFromJSON(cJSON *modification_notificationJSON)
{
    OpenAPI_modification_notification_t *modification_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notify_items = NULL;
    OpenAPI_list_t *notify_itemsList = NULL;
    notify_items = cJSON_GetObjectItemCaseSensitive(modification_notificationJSON, "notifyItems");
    if (!notify_items) {
        ogs_error("OpenAPI_modification_notification_parseFromJSON() failed [notify_items]");
        goto end;
    }
        cJSON *notify_items_local = NULL;
        if (!cJSON_IsArray(notify_items)) {
            ogs_error("OpenAPI_modification_notification_parseFromJSON() failed [notify_items]");
            goto end;
        }

        notify_itemsList = OpenAPI_list_create();

        cJSON_ArrayForEach(notify_items_local, notify_items) {
            if (!cJSON_IsObject(notify_items_local)) {
                ogs_error("OpenAPI_modification_notification_parseFromJSON() failed [notify_items]");
                goto end;
            }
            OpenAPI_notify_item_t *notify_itemsItem = OpenAPI_notify_item_parseFromJSON(notify_items_local);
            if (!notify_itemsItem) {
                ogs_error("No notify_itemsItem");
                OpenAPI_list_free(notify_itemsList);
                goto end;
            }
            OpenAPI_list_add(notify_itemsList, notify_itemsItem);
        }

    modification_notification_local_var = OpenAPI_modification_notification_create (
        notify_itemsList
    );

    return modification_notification_local_var;
end:
    if (notify_itemsList) {
        OpenAPI_list_for_each(notify_itemsList, node) {
            OpenAPI_notify_item_free(node->data);
        }
        OpenAPI_list_free(notify_itemsList);
        notify_itemsList = NULL;
    }
    return NULL;
}

OpenAPI_modification_notification_t *OpenAPI_modification_notification_copy(OpenAPI_modification_notification_t *dst, OpenAPI_modification_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_modification_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_modification_notification_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_modification_notification_free(dst);
    dst = OpenAPI_modification_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

