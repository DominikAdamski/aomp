#include <stdio.h>
#include <omp.h>

int main()
{
  int N = 10;

  int a[N];
  int b[N];

  int i;

  for (i=0; i<N; i++)
    a[i]=0;

  for (i=0; i<N; i++)
    b[i]=i;

#pragma omp target parallel for
  {
    for (int j = 0; j< N; j++)
      a[j]=b[j];
  }

  int rc = 0;
  for (i=0; i<N; i++)
    if (a[i] != b[i] ) {
      rc++;
      printf ("Wrong varlue: a[%d]=%d\n", i, a[i]);
    }

  if (!rc)
    printf("Success\n");

  return rc;
}

// Tool related code below
#include <omp-tools.h>

#define OMPT_BUFFER_REQUEST_SIZE 256

// OMPT entry point handles
static ompt_start_trace_t ompt_start_trace;
static ompt_set_callback_t ompt_set_callback;

// OMPT callbacks

// Trace record callbacks
static void on_ompt_callback_buffer_request (
  int device_num,
  ompt_buffer_t **buffer,
  size_t *bytes
) {
  *bytes = OMPT_BUFFER_REQUEST_SIZE;
  *buffer = malloc(*bytes);
  printf("Allocated %lu bytes at %p in buffer request callback\n", *bytes, *buffer);
}

// Synchronous callbacks
static void on_ompt_callback_device_initialize
(
  int device_num,
  const char *type,
  ompt_device_t *device,
  ompt_function_lookup_t lookup,
  const char *documentation
 ) {
  printf("Invoked on_ompt_callback_device_initialize: %d %s\n", device_num, type);
  ompt_start_trace = (ompt_start_trace_t) lookup("ompt_start_trace");
  printf("ompt_start_trace=%p\n", ompt_start_trace);
  // In many scenarios, this will be a good place to start the
  // trace. If start_trace is called from the main program, the
  // programmer has to be careful to place the call after the first
  // target construct, otherwise the program will fail. This is
  // because this device_init callback is invoked during the first
  // target construct implementation and any start_trace must come
  // afterwards.

  // TODO move the ompt_start_trace to the main program before any
  // target construct and ensure we error out gracefully. The program
  // should not assert or crash.
  int status = ompt_start_trace(0, &on_ompt_callback_buffer_request, 0);
  printf("Start trace status=%d\n", status);
}

static void on_ompt_callback_device_load
    (
     int device_num,
     const char *filename,
     int64_t offset_in_file,
     void *vma_in_file,
     size_t bytes,
     void *host_addr,
     void *device_addr,
     uint64_t module_id
     ) {
  printf("Invoked on_ompt_callback_device_load: %d %s %p %p %lu\n",
     device_num, filename, host_addr, device_addr, bytes);
}

static void on_ompt_callback_target_data_op
    (
     ompt_scope_endpoint_t endpoint,
     ompt_id_t target_id,
     ompt_id_t host_op_id,
     ompt_target_data_op_t optype,
     void *src_addr,
     int src_device_num,
     void *dest_addr,
     int dest_device_num,
     size_t bytes,
     const void *codeptr_ra
     ) {
  printf("Invoked on_ompt_callback_target_data_op: %p %p %lu %p\n",
     src_addr, dest_addr, bytes, codeptr_ra);
}

static void on_ompt_callback_target
    (
     ompt_target_t kind,
     ompt_scope_endpoint_t endpoint,
     int device_num,
     ompt_data_t *task_data,
     ompt_id_t target_id,
     const void *codeptr_ra
     ) {
  printf("Invoked on_ompt_callback_target: %d %lu %p\n",
     device_num, target_id, codeptr_ra);
}

static void on_ompt_callback_target_submit
    (
     ompt_scope_endpoint_t endpoint,
     ompt_id_t target_id,
     ompt_id_t host_op_id,
     unsigned int requested_num_teams
     ) {
  printf("Invoked on_ompt_callback_target_submit: %d %lu %lu %d\n",
     endpoint, target_id, host_op_id, requested_num_teams);
}

// Init functions
int ompt_initialize(
  ompt_function_lookup_t lookup,
  int initial_device_num,
  ompt_data_t *tool_data)
{
  ompt_set_callback = (ompt_set_callback_t) lookup("ompt_set_callback");
  printf("ompt_set_callback=%p\n", ompt_set_callback);
  
  ompt_set_callback(ompt_callback_device_initialize,
		    (ompt_callback_t)&on_ompt_callback_device_initialize);
  ompt_set_callback(ompt_callback_device_load,
		    (ompt_callback_t)&on_ompt_callback_device_load);
  ompt_set_callback(ompt_callback_target_data_op,
		    (ompt_callback_t)&on_ompt_callback_target_data_op);
  ompt_set_callback(ompt_callback_target,
		    (ompt_callback_t)&on_ompt_callback_target);
  ompt_set_callback(ompt_callback_target_submit,
		    (ompt_callback_t)&on_ompt_callback_target_submit);
  return 1; //success
}

void ompt_finalize(ompt_data_t *tool_data)
{
}

#ifdef __cplusplus
extern "C" {
#endif
ompt_start_tool_result_t *ompt_start_tool(
  unsigned int omp_version,
  const char *runtime_version)
{
  static ompt_start_tool_result_t ompt_start_tool_result = {&ompt_initialize,&ompt_finalize, 0};
  return &ompt_start_tool_result;
}
#ifdef __cplusplus
}
#endif
