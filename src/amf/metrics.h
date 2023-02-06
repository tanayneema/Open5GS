#ifndef AMF_METRICS_H
#define AMF_METRICS_H

#include "ogs-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum amf_metric_type_global_s {
    AMF_METR_GLOB_GAUGE_RAN_UE,
    AMF_METR_GLOB_GAUGE_AMF_SESS,
    AMF_METR_GLOB_GAUGE_GNB,
    AMF_METR_GLOB_CTR_RM_REG_INIT_REQ,
    AMF_METR_GLOB_CTR_RM_REG_INIT_SUCC,
    AMF_METR_GLOB_CTR_RM_REG_MOB_REQ,
    AMF_METR_GLOB_CTR_RM_REG_MOB_SUCC,
    AMF_METR_GLOB_CTR_RM_REG_PERIOD_REQ,
    AMF_METR_GLOB_CTR_RM_REG_PERIOD_SUCC,
    AMF_METR_GLOB_CTR_RM_REG_EMERG_REQ,
    AMF_METR_GLOB_CTR_RM_REG_EMERG_SUCC,
    _AMF_METR_GLOB_MAX,
} amf_metric_type_global_t;
extern ogs_metrics_inst_t *amf_metrics_inst_global[_AMF_METR_GLOB_MAX];

int amf_metrics_init_inst_global(void);
int amf_metrics_free_inst_global(void);

static inline void amf_metrics_inst_global_set(amf_metric_type_global_t t, int val)
{ ogs_metrics_inst_set(amf_metrics_inst_global[t], val); }
static inline void amf_metrics_inst_global_add(amf_metric_type_global_t t, int val)
{ ogs_metrics_inst_add(amf_metrics_inst_global[t], val); }
static inline void amf_metrics_inst_global_inc(amf_metric_type_global_t t)
{ ogs_metrics_inst_inc(amf_metrics_inst_global[t]); }
static inline void amf_metrics_inst_global_dec(amf_metric_type_global_t t)
{ ogs_metrics_inst_dec(amf_metrics_inst_global[t]); }

/* BY SLICE */
typedef enum amf_metric_type_by_slice_s {
    AMF_METR_GAUGE_RM_REGISTERED_SUB_NBR = 0,
    _AMF_METR_BY_SLICE_MAX,
} amf_metric_type_by_slice_t;

void amf_metrics_inst_by_slice_add(
    ogs_plmn_id_t *plmn, ogs_s_nssai_t *snssai,
    amf_metric_type_by_slice_t t, int val);

/* BY CAUSE */
typedef enum amf_metric_type_by_cause_s {
    AMF_METR_CTR_RM_REG_INIT_FAIL = 0,
    AMF_METR_CTR_RM_REG_MOB_FAIL,
    AMF_METR_CTR_RM_REG_PERIOD_FAIL,
    AMF_METR_CTR_RM_REG_EMERG_FAIL,
    _AMF_METR_BY_CAUSE_MAX,
} amf_metric_type_by_cause_t;

void amf_metrics_inst_by_cause_add(
    uint8_t cause, amf_metric_type_by_cause_t t, int val);

/* BY REG_TYPE */
typedef enum amf_metric_type_by_reg_type_s {
    AMF_METR_GAUGE_RM_REG_TIME_MEAN = 0,
    AMF_METR_GAUGE_RM_REG_TIME_MAX,
    _AMF_METR_BY_REG_TYPE_MAX,
} amf_metric_type_by_reg_type_t;

void amf_metrics_inst_by_reg_type_set(
    int reg_type, amf_metric_type_by_reg_type_t t, int val);

void amf_metrics_reg_time_start(amf_ue_t *amf_ue);
int amf_metrics_reg_time_stop(amf_ue_t *amf_ue);
void amf_metrics_reg_time_add(int reg_req_ms, uint8_t reg_type);
void amf_metrics_reg_time_expose(void);

void amf_metrics_init(void);
void amf_metrics_final(void);

#ifdef __cplusplus
}
#endif

#endif /* AMF_METRICS_H */
