import { Message } from "@/components/UI"
import { useChatStore } from "@/state/chat"
import { useUserStore } from "@/state/user"
import { IMessage } from "@/types"
import cls from "@/utils/cls"
import { forwardRef } from "react"
import styles from "./Chat.module.css"

interface ChatProps {
    className?: string
    ref?: React.Ref<HTMLDivElement>
}

export const Chat = forwardRef<HTMLDivElement, ChatProps>(({ className = "" }, ref) => {
    const currentUser = useUserStore((state) => state.data)
    const messages = useChatStore((state) => state.messages)
    const currentChat = useChatStore((state) => state.data)

    return (
        <div className={cls(className, styles.chat)} ref={ref}>
            {currentChat ? (
                messages.length > 0 ? (
                    <ul className={styles.messages}>
                        {[...messages].reverse().map(
                            (item: IMessage, index: number): JSX.Element => (
                                <Message
                                    key={index}
                                    variant={item.authorId === currentUser?.id ? "right" : "left"}
                                >
                                    {item.body}
                                </Message>
                            )
                        )}
                    </ul>
                ) : (
                    <div className={styles.empty}>No messages</div>
                )
            ) : (
                <div className={styles.empty}>Select a chat</div>
            )}
        </div>
    )
})
