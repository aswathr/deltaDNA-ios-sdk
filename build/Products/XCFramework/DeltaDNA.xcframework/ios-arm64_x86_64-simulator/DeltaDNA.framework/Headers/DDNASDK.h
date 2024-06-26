//
// Copyright (c) 2016 deltaDNA Ltd. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#import <Foundation/Foundation.h>
#import "DDNALog.h"
#import "DDNAEventAction.h"

@class DDNASettings;
@class DDNAEvent;
@class DDNAEngagement;
@class DDNAEngageFactory;
@class DDNAUserManager;
@class DDNAConsentTracker;
@protocol DDNAPopup;

@protocol DDNASDKDelegate;

@interface DDNASDK : NSObject

@property (nonatomic, strong) id<DDNASdkInterface> impl;
@property (nonatomic, strong) DDNAUserManager *userManager;

/**
 INTERNAL USE ONLY. Do not use this property directly. Use the methods isPiplConsentRequired and
 setPiplConsentForDataUse to check for and track a user's PIPL consent.
 */
@property (nonatomic, strong) DDNAConsentTracker *consentTracker;

@property (nonatomic, weak) id<DDNASDKDelegate> delegate;

/**
 Change default SDK behaviour via the settings property.
 @see See DDNASettings.h for available options.
 */
@property (nonatomic, strong) DDNASettings *settings;

/** 
 The hash secret for your environment.  This must be set
 @b before starting the SDK.  You only need to set this
 if hashing as been enabled for this environment.  If 
 hashing is enabled and the secret has not been set Collect
 will return 403 errors.
 */
@property (nonatomic, copy) NSString *hashSecret;

/**
 A version string for your game.  This is used to help you
 identify which version of your game is being played on the
 platform.  This must be set @b before starting the SDK.
 */
@property (nonatomic, copy) NSString *clientVersion;

/**
 The cross game user ID. This is used for cross promotion
 and should be set once the user signs into a service which
 uniquely identifies the user.
 */
@property (nonatomic, copy) NSString *crossGameUserId;

/**
 The Apple developer ID email of the developer account that published
 the current application. (e.g. test@example.com). This needs to be set
 before any Audience Pinpointer methods are called.
 */
@property NSString *appleDeveloperId;

/**
 The iTunes connect store ID of the current application. This needs to be set
 before any Audience Pinpointer methods are called.
 */
@property NSString *appStoreId;

/**
 The Apple push notification token. Set this @b before starting
 the SDK to enable DeltaDNA to send push notifications to your
 game.

 @see https://docs.deltadna.com/advanced-integration/ios-sdk/#push-notifications for
 an example of how to get the token.
 */

@property (nonatomic, copy) NSString *pushNotificationToken __attribute__((deprecated("Replaced by setDeviceToken - Please see README for updated usage.")));

/**
The Apple Device Token received from in your AppDelegate
 didRegisterForRemoteNotificationsWithDeviceToken.
 Set this @b before starting the SDK to enable DeltaDNA
 to send push notifications to your game.
 */
@property (nonatomic, copy) NSData *deviceToken;
/// The Unity ProjectID
@property (nonatomic, copy, readonly) NSString *projectID;
/// The environment name for this game environment (Dev or Live).
@property (nonatomic, copy, readonly) NSString *environmentName;
/// The environment key for this game environment (Dev or Live).
@property (nonatomic, copy, readonly) NSString *environmentKey;
/// The URL for Collect for this environment.
@property (nonatomic, copy, readonly) NSString *collectURL;
/// The URL for Engage for this environment.
@property (nonatomic, copy, readonly) NSString *engageURL;
/// The User ID for this game.
@property (nonatomic, copy, readonly) NSString *userID;
/// The Session ID for this game.
@property (nonatomic, copy, readonly) NSString *sessionID;
/// The platform this game is running on.
@property (nonatomic, copy) NSString *platform;

/// Has the SDK been started yet.
@property (nonatomic, assign, readonly, getter = hasStarted) BOOL started;

/// Is the SDK uploading events.
@property (nonatomic, assign, readonly, getter = isUploading) BOOL uploading;

/// The @c DDNAEngageFactory helps making Engage requests.
@property (nonatomic, strong, readonly) DDNAEngageFactory *engageFactory;

/**
 Singleton access to the deltaDNA SDK.
 
 @return The deltaDNA SDK instance.
 */
+ (instancetype)sharedInstance;

/**
 The SDK must be started once before you can send events.
 @param projectID The Id of the Unity Project
 
 @param environmentName The games's unique environment Name. e.g. "production"
 
 @param environmentKey The games's unique environment Key e.g. "1875d8f6-5c64-42e0-9e18-be437a0a126d"
 
 @param analyticsURL The Unity Analytics URL.
 
 @param remoteConfigURL The Remote Config URL, use nil if not using Engage.
 */
- (void)startWithProjectID: (NSString *) projectID
           environmentName: (NSString *) environmentName
            environmentKey: (NSString *) environmentKey
              analyticsURL: (NSString *) analyticsURL
           remoteConfigURL: (NSString *) remoteConfigURL;


/**
 The SDK must be started once before you can send events.
 @param projectID The Id of the Unity Project
 
 @param environmentName The games's unique environment Name. e.g. "production"
 
 @param environmentKey The games's unique environment Key e.g. "1875d8f6-5c64-42e0-9e18-be437a0a126d"
 
 @param analyticsURL The Unity Analytics URL.
 
 @param remoteConfigURL The Remote Config URL, use nil if not using Engage.
 
 @param userID The user id to associate the game events with, use nil if you want the SDK to generate a random one.
 */
- (void)startWithProjectID: (NSString *) projectID
           environmentName: (NSString *) environmentName
            environmentKey: (NSString *) environmentKey
              analyticsURL: (NSString *) analyticsURL
           remoteConfigURL: (NSString *) remoteConfigURL
                    userID: (NSString *) userID;


/**
 Generates a new session id, subsequent events will belong to a new session.
 
 @discussion New sessions are generated automatically if the app leaves the foreground and returns after a period of time (default: 5 minutes).  This behaviour can be configured in the settings.
 */
- (void)newSession;

/**
 Sends a 'gameEnded' event to Collect and stops background uploads.
 */
- (void)stop;

/**
 Records an event using the DDNAEvent builder class.
 
 @param event The event to record.
 
 @exception Throws @c DDNANotStartedException if @c -startWithEnvironmentKey: has not been called.
 */
- (DDNAEventAction *)recordEvent:(DDNAEvent *)event;

/**
 Records an event with no custom parameters.
 
 @param eventName The name of the event schema.
 
 @exception Throws @c DDNANotStartedException if @c -startWithEnvironmentKey: has not been called.
 */
- (DDNAEventAction *)recordEventWithName:(NSString *)eventName;

/**
 Records an event with a dictionary of event parameters.  Structure the dictionary keys to match the @b eventParams structure of your event schema.
 
 @param eventName The name of the event schema.
 
 @param eventParams A dictionary of event parameters.
 
 @exception Throws @c DDNANotStartedException if @c -startWithEnvironmentKey: has not been called.
 */
- (DDNAEventAction *)recordEventWithName:(NSString *)eventName eventParams:(NSDictionary *)eventParams;

/**
 Makes an Engage call.  Create a @c DDNAEngagement with a decision point and optional parameters.  If the engagement is recognised by the platform the completion handler returns the set of parameters to use.
 
 @param engagement The engagement to request.
 
 @param completionHandler Optional callback that reports the response the engagement returns.  The status code and error report any network failures.
 
 @exception Throws @c DDNANotStartedException if @c -startWithEnvironmentKey: has not been called. Throws @c NSInvalidArgumentException if the engage URL has not been set or the parameters are nil.
 */
- (void)requestEngagement:(DDNAEngagement *)engagement
        completionHandler:(void(^)(NSDictionary *response, NSInteger statusCode, NSError *error))completionHandler;

/**
 Request an engagement from Engage.  Create a @c DDNAEngagement with a decision point and optional parameters.  The engagementHandler is called once the request has completed.  If the engagement succeeded the @c -json property will no longer be nil and the <i> parameters</i> value should be inspected to see which parameters if any were returned.  Otherise the @c -error property will report why the engagement failed.  @see @c DDNAEngagement for more details.
 
 @param engagement The engagement to send to Engage.
 
 @param engagementHandler The block that is called with the popuplated engagement once the request has completed.
 
 @exception Throws @c DDNANotStartedException if @c -startWithEnvironmentKey: has not been called.  Throws @c NSInvalidArgumentException if the engage URL has not been set or the parameters are nil.
 */
- (void)requestEngagement:(DDNAEngagement *)engagement engagementHandler:(void(^)(DDNAEngagement *))engagementHandler;

/**
 Records receiving a push notification.  Call from @c application:didFinishLaunchingWithOptions and @c application:didReceiveRemoteNotification so we can track the open rate of your notifications.  It is safe to call this method before @c startWithEnvironmentKey:collectURL:engageURL, the event will be queued.
 */
- (void) recordPushNotification: (NSDictionary *) pushNotification
                      didLaunch: (BOOL) didLaunch;

/**
 Sends recorded events to deltaDNA.  The default SDK behaviour is to call this
 periodically in the background for you.  If you disable background uploading
 you must call this method regularly to send your game events.  The call is
 non blocking.
 */
- (void)upload;

/**
 Makes a session configuration request.  This will occur automatically on a new session, but can
 be called here if more control is required.
 
 The result will be notified from the @c DDNASDKDelegate.
 */
- (void)requestSessionConfiguration;

/**
 Downloads image assets from the session configuration in the background.
 
 This happens automatically whenever a session configuration request takes
 but can be called explicity if more control is required.
 
 The result will be notified from the @c DDNASDKDelegate.
 */
- (void)downloadImageAssets;

/**
 Clears persisted data from the device.  This includes any cached events that
 haven't been sent to Collect, cached engagement request responses and the 
 user id.  If the user id was auto generated by the SDK, a new user id will
 be created next time the game runs.  The newPlayer event will also be sent
 again.
 */
- (void)clearPersistentData;

/**
 Changes the default log level from warning.
 
 @param logLevel The log level to set.
 */
+ (void)setLogLevel:(DDNALogLevel)logLevel;

/**
 Notifies our platform that this user id should be forgotten inline with GDPR regulations.
 Once called, StartSDK will no longer function unless started with a different user id.
 */
- (void)forgetMe;

#pragma mark Pinpointer Methods

/**
 Records a session event to be used in pinpointer signal tracking.
 Only designed to be used for Audience Pinpointer.
 @availability iOS 12 or higher
 */
- (void) recordSignalTrackingSessionEvent;

/**
 Records a installation event to be used in pinpointer signal tracking.
 Only designed to be used for Audience Pinpointer.
 @availability iOS 12 or higher
 */
- (void) recordSignalTrackingInstallEvent;

/**
 Records a purchase event to be used in pinpointer signal tracking.
 Only designed to be used for Audience Pinpointer.
 @availability iOS 12 or higher
 @param realCurrencyAmount The amount spent on the purchase, in the currency used for the purchase
 @param realCurrencyType The currency code of the currency used for the purchase. For example, USD for dollars or GBP for pounds sterling.
 @param transactionID The transaction ID of the purchase, as reported by Apple's StoreKit API
 @param transactionReceipt The base64 encoded receipt data, as returned by Apple's StoreKit validation API
 */
- (void) recordSignalTrackingPurchaseEventWithRealCurrencyAmount :(NSNumber *)realCurrencyAmount realCurrencyType:(NSString *)realCurrencyType transactionID:(NSString *)transactionID transactionReceipt:(NSString *)transactionReceipt;

// MARK: PIPL Consent

/**
 Checks to see if PIPL consent is required to send and record data.
 As of version 5.0.0 and higher, this method _must_ be called before events will be sent from the device.
 @param callback A callback which receives a boolean value - true if consent is required, and false if not (or if an error occurred), and an optional NSError. If the NSError is not nil, then a problem occurred while checking for required consents, and you should try again later.
 */
- (void) isPiplConsentRequired :(void(^)(BOOL, NSError *))callback;

/**
 Sets the user's preference for PIPL consent, if required.
 Note: You should call isPiplConsentRequired first in order to determine if the user is in a jurisdiction that requires PIPL compliance.
 @param dataUse Whether or not the user has granted consent for their data to be used
 @param dataExport Whether or not the user has granted consent for their data to be exported from China
 */
- (void) setPiplConsentForDataUse :(BOOL)dataUse andDataExport:(BOOL)dataExport;

@end

/**
 Implement the @c DDNASDKDelegate methods to be informed of actions in deltaDNA's sdk.
 */
@protocol DDNASDKDelegate <NSObject>

@optional

/**
 Called when the sdk has been started.
 */
- (void)didStartSdk;

/**
 Called when the sdk has been stopped.
 */
- (void)didStopSdk;

/**
 Called when the session configuration has completed.
 
 @param cache. If the local cache was used due to lack of network.
 */
- (void)didConfigureSessionWithCache:(BOOL)cache;

/**
 Called when no session configuration was available, either live or cached.
 
 @param error. Any error that may have been reported by the network.
 */
- (void)didFailToConfigureSessionWithError:(NSError *)error;

/**
 Called then the image cache has been fully populated.  This means all @c DDNAImageMessage used
 by the game will load without a network connection.
 */
- (void)didPopulateImageMessageCache;

/**
 Called when we've not been able to fully populate the image cache.  This means @c DDNAImageMessage used by
 event-triggered campaigns may not operate.
 */
- (void)didFailToPopulateImageMessageCacheWithError:(NSError *)error;

@end

