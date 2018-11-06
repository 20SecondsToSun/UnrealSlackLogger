// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "SlackLoggerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALSLACK_API USlackLoggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USlackLoggerComponent();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Network")
	FString SlackUrl = "https://hooks.slack.com/services/...";

	UFUNCTION(BlueprintCallable, Category = "Network")
	void Post(const FString& slackMessage);

protected:
	virtual void BeginPlay() override;

private:
	FHttpModule* Http;
	void SlackResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);	
	TSharedRef<IHttpRequest> PostRequest(const FString& ContentJsonString);
	FString MakeSlackFormatMessage(const FString& Message) const;
};
