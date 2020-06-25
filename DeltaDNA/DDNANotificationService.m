#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <UserNotifications/UserNotifications.h>
#import "DDNANotificationService.h"

@interface DDNANotificationService ()

@property (nonatomic, strong) void (^contentHandler)(UNNotificationContent *contentToDeliver);
@property (nonatomic, strong) UNMutableNotificationContent *bestAttemptContent;

@end

@implementation DDNANotificationService

- (void)didReceiveNotificationRequest:(UNNotificationRequest *)request withContentHandler:(void (^)(UNNotificationContent * _Nonnull))contentHandler {
    self.contentHandler = contentHandler;
    self.bestAttemptContent = [request.content mutableCopy];
    
    NSDictionary *userInfo = [request.content.userInfo objectForKey:@"aps"];
    NSString *imageUrl = [userInfo objectForKey:@"imageUrl"];
    
    if (imageUrl == nil) {
        [self contentComplete];
        return;
    }
    
    [self loadAttachmentForUrlString:imageUrl completionHandler:^(UNNotificationAttachment *attachment) {
        if (attachment) {
            self.bestAttemptContent.attachments = [NSArray arrayWithObject:attachment];
        }
        [self contentComplete];
    }];
}

-(void)contentComplete {
    self.contentHandler(self.bestAttemptContent);
}

- (void)serviceExtensionTimeWillExpire {
    [self contentComplete];
}

-(void)loadAttachmentForUrlString:(NSString *)urlString completionHandler:(void(^)(UNNotificationAttachment *))completionHandler {
    
    __block UNNotificationAttachment *attachment = nil;
    NSString *fileExt = [@"." stringByAppendingString:[urlString pathExtension]];
    NSURL *attachmentUrl = [NSURL URLWithString:urlString];
    
    NSURLSession *session = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration]];
    
    [[session downloadTaskWithURL:attachmentUrl
                completionHandler:^(NSURL *temporaryFileLocation, NSURLResponse *response, NSError *error) {
        if (error != nil) {
            NSLog(@"%@", error.localizedDescription);
        } else {
            NSFileManager *fileManager = [NSFileManager defaultManager];
            NSURL *localUrl = [NSURL fileURLWithPath:[temporaryFileLocation.path stringByAppendingString:fileExt]];
            [fileManager moveItemAtURL:temporaryFileLocation toURL:localUrl error:&error];
            
            NSError *attachmentError = nil;
            attachment = [UNNotificationAttachment attachmentWithIdentifier:@"" URL:localUrl options:nil error:&attachmentError];
            if (attachmentError) {
                NSLog(@"%@", attachmentError.localizedDescription);
            }
        }
        completionHandler(attachment);
    }] resume];
}

@end
