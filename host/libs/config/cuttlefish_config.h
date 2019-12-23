/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <memory>
#include <string>
#include <set>

namespace Json {
class Value;
}

namespace vsoc {

constexpr char kDefaultUuidPrefix[] = "699acfc4-c8c4-11e7-882b-5065f31dc1";
constexpr char kCuttlefishConfigEnvVarName[] = "CUTTLEFISH_CONFIG_FILE";
constexpr char kVsocUserPrefix[] = "vsoc-";
constexpr char kBootStartedMessage[] ="VIRTUAL_DEVICE_BOOT_STARTED";
constexpr char kBootCompletedMessage[] = "VIRTUAL_DEVICE_BOOT_COMPLETED";
constexpr char kBootFailedMessage[] = "VIRTUAL_DEVICE_BOOT_FAILED";
constexpr char kMobileNetworkConnectedMessage[] =
    "VIRTUAL_DEVICE_NETWORK_MOBILE_CONNECTED";
constexpr char kWifiConnectedMessage[] =
    "VIRTUAL_DEVICE_NETWORK_WIFI_CONNECTED";
constexpr char kInternalDirName[] = "internal";


enum class AdbMode {
  VsockTunnel,
  VsockHalfTunnel,
  NativeVsock,
  Unknown,
};

// Holds the configuration of the cuttlefish instances.
class CuttlefishConfig {
 public:
  static const CuttlefishConfig* Get();

  CuttlefishConfig();
  ~CuttlefishConfig();

  // Saves the configuration object in a file, it can then be read in other
  // processes by passing the --config_file option.
  bool SaveToFile(const std::string& file) const;

  // Returns the path to a file with the given name in the instance directory..
  std::string PerInstancePath(const char* file_name) const;
  std::string PerInstanceInternalPath(const char* file_name) const;

  std::string instance_name() const;

  std::string instance_dir() const;
  void set_instance_dir(const std::string& instance_dir);

  std::string instance_internal_dir() const;

  std::string vm_manager() const;
  void set_vm_manager(const std::string& name);

  std::string gpu_mode() const;
  void set_gpu_mode(const std::string& name);

  std::string wayland_socket() const;
  void set_wayland_socket(const std::string& path);

  std::string x_display() const;
  void set_x_display(const std::string& address);

  std::string serial_number() const;
  void set_serial_number(const std::string& serial_number);

  int cpus() const;
  void set_cpus(int cpus);

  int memory_mb() const;
  void set_memory_mb(int memory_mb);

  int dpi() const;
  void set_dpi(int dpi);

  int x_res() const;
  void set_x_res(int x_res);

  int y_res() const;
  void set_y_res(int y_res);

  int refresh_rate_hz() const;
  void set_refresh_rate_hz(int refresh_rate_hz);

  // Returns kernel image extracted from the boot image or the user-provided one
  // if given by command line to the launcher. This function should not be used
  // to get the kernel image the vmm should boot, GetKernelImageToUse() should
  // be used instead.
  std::string kernel_image_path() const;
  void set_kernel_image_path(const std::string& kernel_image_path);

  bool decompress_kernel() const;
  void set_decompress_kernel(bool decompress_kernel);

  // Returns the path to the kernel image that should be given to the vm manager
  // to boot, takes into account whether the original image was decompressed or
  // not.
  std::string GetKernelImageToUse() const {
    return decompress_kernel() ? decompressed_kernel_image_path()
                               : kernel_image_path();
  }

  std::string decompressed_kernel_image_path() const;
  void set_decompressed_kernel_image_path(const std::string& path);

  bool use_unpacked_kernel() const;
  void set_use_unpacked_kernel(bool use_unpacked_kernel);

  std::string gdb_flag() const;
  void set_gdb_flag(const std::string& gdb);

  std::string ramdisk_image_path() const;
  void set_ramdisk_image_path(const std::string& ramdisk_image_path);

  std::string initramfs_path() const;
  void set_initramfs_path(const std::string& initramfs_path);

  std::string final_ramdisk_path() const;
  void set_final_ramdisk_path(const std::string& final_ramdisk_path);

  std::string vendor_ramdisk_image_path() const;
  void set_vendor_ramdisk_image_path(const std::string&
    vendor_ramdisk_image_path);

  std::vector<std::string> virtual_disk_paths() const;
  void set_virtual_disk_paths(const std::vector<std::string>& disk_paths);

  std::string kernel_log_pipe_name() const;

  std::string console_pipe_name() const;

  bool deprecated_boot_completed() const;
  void set_deprecated_boot_completed(bool deprecated_boot_completed);

  std::string console_path() const;

  std::string logcat_path() const;

  std::string logcat_receiver_binary() const;
  void set_logcat_receiver_binary(const std::string& binary);

  std::string config_server_binary() const;
  void set_config_server_binary(const std::string& binary);

  std::string launcher_log_path() const;

  std::string launcher_monitor_socket_path() const;

  std::string mobile_bridge_name() const;
  void set_mobile_bridge_name(const std::string& mobile_bridge_name);

  std::string mobile_tap_name() const;
  void set_mobile_tap_name(const std::string& mobile_tap_name);

  std::string wifi_tap_name() const;
  void set_wifi_tap_name(const std::string& wifi_tap_name);

  void set_vsock_guest_cid(int vsock_guest_cid);
  int vsock_guest_cid() const;

  std::string uuid() const;
  void set_uuid(const std::string& uuid);

  void set_cuttlefish_env_path(const std::string& path);
  std::string cuttlefish_env_path() const;

  void set_adb_mode(const std::set<std::string>& modes);
  std::set<AdbMode> adb_mode() const;

  void set_host_port(int host_port);
  int host_port() const;

  void set_adb_ip_and_port(const std::string& ip_port);
  std::string adb_ip_and_port() const;

  std::string adb_device_name() const;

  void set_device_title(const std::string& title);
  std::string device_title() const;

  void set_setupwizard_mode(const std::string& title);
  std::string setupwizard_mode() const;

  void set_qemu_binary(const std::string& qemu_binary);
  std::string qemu_binary() const;

  void set_crosvm_binary(const std::string& crosvm_binary);
  std::string crosvm_binary() const;

  void set_console_forwarder_binary(const std::string& crosvm_binary);
  std::string console_forwarder_binary() const;

  void set_kernel_log_monitor_binary(
      const std::string& kernel_log_monitor_binary);
  std::string kernel_log_monitor_binary() const;

  void set_enable_vnc_server(bool enable_vnc_server);
  bool enable_vnc_server() const;

  void set_vnc_server_port(int vnc_server_port);
  int vnc_server_port() const;

  void set_vnc_server_binary(const std::string& vnc_server_binary);
  std::string vnc_server_binary() const;

  void set_restart_subprocesses(bool restart_subprocesses);
  bool restart_subprocesses() const;

  void set_run_adb_connector(bool run_adb_connector);
  bool run_adb_connector() const;

  void set_adb_connector_binary(const std::string& adb_connector_binary);
  std::string adb_connector_binary() const;

  void set_socket_vsock_proxy_binary(const std::string& binary);
  std::string socket_vsock_proxy_binary() const;

  void set_run_as_daemon(bool run_as_daemon);
  bool run_as_daemon() const;

  void set_data_policy(const std::string& data_policy);
  std::string data_policy() const;

  void set_blank_data_image_mb(int blank_data_image_mb);
  int blank_data_image_mb() const;

  void set_blank_data_image_fmt(const std::string& blank_data_image_fmt);
  std::string blank_data_image_fmt() const;

  void set_logcat_mode(const std::string& mode);
  std::string logcat_mode() const;

  void set_enable_tombstone_receiver(bool enable_tombstone_receiver);
  bool enable_tombstone_receiver() const;

  void set_tombstone_receiver_binary(const std::string& binary);
  std::string tombstone_receiver_binary() const;

  void set_use_bootloader(bool use_bootloader);
  bool use_bootloader() const;

  void set_bootloader(const std::string& bootloader_path);
  std::string bootloader() const;

  void set_boot_slot(const std::string& boot_slot);
  std::string boot_slot() const;

  std::string touch_socket_path() const;
  std::string keyboard_socket_path() const;

  void set_loop_max_part(int loop_max_part);
  int loop_max_part() const;

  void set_guest_enforce_security(bool guest_enforce_security);
  bool guest_enforce_security() const;

  void set_guest_audit_security(bool guest_audit_security);
  bool guest_audit_security() const;

  void set_boot_image_kernel_cmdline(std::string boot_image_kernel_cmdline);
  std::vector<std::string> boot_image_kernel_cmdline() const;

  void set_extra_kernel_cmdline(std::string extra_cmdline);
  std::vector<std::string> extra_kernel_cmdline() const;

 private:
  std::unique_ptr<Json::Value> dictionary_;

  void SetPath(const std::string& key, const std::string& path);
  bool LoadFromFile(const char* file);
  static CuttlefishConfig* BuildConfigImpl();

  CuttlefishConfig(const CuttlefishConfig&) = delete;
  CuttlefishConfig& operator=(const CuttlefishConfig&) = delete;
};

// Returns the instance number as obtained from the CUTTLEFISH_INSTANCE
// environment variable or the username.
int GetInstance();
// Returns a path where the launhcer puts a link to the config file which makes
// it easily discoverable regardless of what vm manager is in use
std::string GetGlobalConfigFileLink();

// These functions modify a given base value to make it different accross
// different instances by appending the instance id in case of strings or adding
// it in case of integers.
std::string GetPerInstanceDefault(const char* prefix);
int GetPerInstanceDefault(int base);

std::string GetDefaultPerInstanceDir();
std::string GetDefaultMempath();
int GetDefaultPerInstanceVsockCid();

std::string DefaultHostArtifactsPath(const std::string& file);
std::string DefaultGuestImagePath(const std::string& file);
std::string DefaultEnvironmentPath(const char* environment_key,
                                   const char* default_value,
                                   const char* path);

// Whether the host supports qemu
bool HostSupportsQemuCli();
bool HostSupportsVsock();

// GPU modes
extern const char* const kGpuModeGuestSwiftshader;
extern const char* const kGpuModeDrmVirgl;
}  // namespace vsoc