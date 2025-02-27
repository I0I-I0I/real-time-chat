import { useId } from "react"
import styles from "./MessagePrompt.module.css"

import { Button, Input, Label } from "@/components/UI"
import cls from "@/utils/cls"
import { ReactSVG } from "react-svg"

import { useChatStore } from "@/state/chat"
import MessageService from "@/api/MessageService"

interface MessagePromptProps {
    className?: string
}

export const MessagePrompt = ({
    className = ""
}: MessagePromptProps): JSX.Element => {
    const id = useId()
    const chat = useChatStore(state => state.data)
    const user = useChatStore(state => state.data)

    const OnChooseFileClick = (e: React.MouseEvent) => {
        e.preventDefault()
        alert("In development")
    }

    const onSendMessage = async (e: React.FormEvent<HTMLFormElement>) => {
        console.log(chat)
        e.preventDefault()
        if (chat === null) return
        if (user === null) return
        const data = await MessageService.createOne({
            id: 0,
            chatId: chat.id,
            authorId: user.id,
            text: "Hello",
            createdAt: ""
        })
        console.log(data)
    }

    return (
        <div className={cls(styles.message_prompt, className)}>
            <form action="" onSubmit={onSendMessage}>
                <Label htmlFor={id} variant="file" onClick={OnChooseFileClick}>
                    <ReactSVG
                        className={styles.icon}
                        src="/file.svg"
                    />
                    <Input id={id} type="file" variant="file" />
                </Label>
                <Input type="text" variant="message_input" placeholder="Message..." className={styles.prompt} />
                <Button type="submit" variant="send">Send</Button>
            </form>
        </div>
    )
}
